#include "ofxEffectsSoundPlayer.h"


//--------------------------------------------------------------
//CLASS ofxEffectsSoundPlayer
//--------------------------------------------------------------
void ofxEffectsSoundPlayer::Setup()
{
	//Initial system
	this->m_oResult = FMOD::System_Create(&this->m_oSys);
	this->ErrorCheck();

	this->m_oSys->init(32, FMOD_INIT_NORMAL, 0);
	this->ErrorCheck();

	//Create DSP
	this->m_oResult = this->m_oSys->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &this->m_oDSPHighPass);
	this->ErrorCheck();
	this->m_oDSPHighPass->setParameter(FMOD_DSP_HIGHPASS_RESONANCE, 2.8);

	this->m_oResult = this->m_oSys->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &this->m_oDSPLowPass);
	this->ErrorCheck();
	this->m_oDSPLowPass->setParameter(FMOD_DSP_LOWPASS_RESONANCE, 2.8);

	this->m_oResult = this->m_oSys->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &this->m_oDSPDistortion);
	this->ErrorCheck();
	this->m_oResult = this->m_oSys->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &this->m_oDSPPitchShift);
	this->ErrorCheck();

	this->m_bIsSetup = true;

	//Cheat Mode
	this->m_bEnableCheatMode = false;
	this->m_iCheatIndex = 0;	
}
//--------------------------------------------------------------
void ofxEffectsSoundPlayer::Update()
{
	if(this->m_bIsSetup && this->m_bIsLoadSuccess)
	{
		this->m_oSys->update();

		if(this->m_bIsRoll)
		{
			unsigned long long uintDiff_ = (ofGetElapsedTimeMicros() - this->m_uint64MicroTimer);
			if( uintDiff_ > this->m_uint64RollMicroTime )
			{	
				this->m_uint64MicroTimer = ofGetElapsedTimeMicros();
				//this->m_oChannel->setPosition(this->m_uintRollTriggerSoundTimeMS, FMOD_TIMEUNIT_MS);
				this->m_oChannel->setPosition(this->m_uintRollStartTimeMS, FMOD_TIMEUNIT_MS);
			}
		}
	}
}
//--------------------------------------------------------------
void ofxEffectsSoundPlayer::Exit()
{
	this->m_oSound->release();
	this->m_oSys->close();
	this->m_oSys->release();
}

//--------------------------------------------------------------
bool ofxEffectsSoundPlayer::LoadSounds(std::string strFilePath, bool bLoop, float fBPM)
{
	if(!this->m_bIsSetup)
	{
		this->Setup();
	}

	//Load sound file
	this->m_oResult = this->m_oSys->createSound(strFilePath.c_str(), FMOD_SOFTWARE, 0, &this->m_oSound);
	this->ErrorCheck();

	if(bLoop)
	{
		this->m_oSound->setMode(FMOD_LOOP_NORMAL);
	}

	//Get sound length
	this->m_oResult = this->m_oSound->getLength(&this->m_uintSoundLength, FMOD_TIMEUNIT_MS);
	this->ErrorCheck();

	//Setup roll 
	this->m_uint64RollTriggerSysTimeMS = 0;
	this->m_bIsRoll = 0;
	this->m_fMicroSPerBeat = (60.0/fBPM) * 1000000;
	this->m_fMicroSHalfBeat = (this->m_fMicroSPerBeat/2.0);
	this->m_fMicroSOneQuarteBeat = (this->m_fMicroSPerBeat/4.0);

	this->m_fMSPerBeat = (60.0/fBPM) * 1000;
	this->m_fMSHalfBeat = (this->m_fMSPerBeat/2.0);
	this->m_fMSOneQuarteBeat = (this->m_fMSPerBeat/4.0);
	this->SetupRoll();

	this->m_uintRollTriggerSoundTimeMS = 0;
	this->m_uintRollStartTimeMS = 0;
	

	if(this->m_oResult == FMOD_OK)
	{
		this->m_bIsLoadSuccess = true;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::Play()
{
	if(this->m_bIsLoadSuccess)
	{
		if(!this->IsPlaying())
		{
			this->m_oResult = this->m_oSys->playSound(FMOD_CHANNEL_FREE, this->m_oSound, false, &this->m_oChannel);
			this->ErrorCheck();
		}
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::Pause()
{
	if(this->m_bIsLoadSuccess)
	{
		this->m_oChannel->setPaused(!this->IsPauseing());
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::Stop()
{
	if(this->m_bIsLoadSuccess && this->IsPlaying())
	{
		if(this->IsPlaying())
		{
			this->m_oResult = this->m_oChannel->stop();
			this->ErrorCheck();
		}
	}
}

//--------------------------------------------------------------
unsigned int ofxEffectsSoundPlayer::GetPosition()
{
	if(this->m_bIsRoll)
	{
		unsigned long long uint64DiffMS_ = ofGetElapsedTimeMillis() - this->m_uint64RollTriggerSysTimeMS;
		return (this->m_uintRollTriggerSoundTimeMS + uint64DiffMS_);
	}
	else
	{
		unsigned int uiPosition_;
		this->m_oChannel->getPosition(&uiPosition_, FMOD_TIMEUNIT_MS);
		return uiPosition_;
	}
}

//--------------------------------------------------------------
float ofxEffectsSoundPlayer::GetPercentagePosition()
{
	unsigned int uiPosition_ = this->GetPosition();
	
	return (float)uiPosition_ / (float)this->m_uintSoundLength;
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetPosition(unsigned int uiPosition)
{
	if(this->m_bIsLoadSuccess)
	{
		if(uiPosition >= this->m_uintSoundLength)
		{
			ofLogWarning("Setting position is longer than sound length");
		}
		else
		{
			this->m_oChannel->setPosition(uiPosition, FMOD_TIMEUNIT_MS);
		}
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetVolume(float fVol)
{
	if(this->m_bIsLoadSuccess)
	{
		fVol = MAX(fVol, 0);
		fVol = MIN(fVol, 1);
		this->m_oChannel->setVolume(fVol);
	}
}

//--------------------------------------------------------------
bool ofxEffectsSoundPlayer::IsPlaying()
{
	bool bIsPlaying_ = false;
	this->m_oChannel->isPlaying(&bIsPlaying_);

	return bIsPlaying_;
}

//--------------------------------------------------------------
bool ofxEffectsSoundPlayer::IsPauseing()
{
	bool bIsPaused_ = false;
	this->m_oChannel->getPaused(&bIsPaused_);

	return bIsPaused_;
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::ApplyReverb()
{
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_GENERIC;
	this->m_oSys->setReverbProperties(&prop);
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetHighPass(bool bApply)
{
	//Default:5000.0
	this->SetHighPass(bApply, 5000.0);
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetHighPass(bool bApply, float fCutOffHz)
{
	bool bActive_;
	this->m_oDSPHighPass->getActive(&bActive_);

	if(bApply)
	{
		if(!bActive_)
		{
			this->m_oSys->addDSP(this->m_oDSPHighPass, 0);
		}
		
		fCutOffHz = MIN(fCutOffHz, 22000.0);
		fCutOffHz = MAX(fCutOffHz, 10.0);
		
		this->m_oDSPHighPass->setParameter(FMOD_DSP_HIGHPASS_CUTOFF, fCutOffHz);
	}
	else
	{
		if(bActive_)
		{
			this->m_oDSPHighPass->remove();
		}
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetLowPass(bool bApply)
{
	//Default:5000.0
	this->SetLowPass(bApply, 5000.0);
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetLowPass(bool bApply, float fCutOffHz)
{
	bool bActive_;
	this->m_oDSPLowPass->getActive(&bActive_);

	if(bApply)
	{
		if(!bActive_)
		{
			this->m_oSys->addDSP(this->m_oDSPLowPass, 0);
		}
		
		fCutOffHz = MIN(fCutOffHz, 22000.0);
		fCutOffHz = MAX(fCutOffHz, 10.0);

		this->m_oDSPLowPass->setParameter(FMOD_DSP_LOWPASS_CUTOFF, fCutOffHz);
	}
	else
	{
		if(bActive_)
		{
			this->m_oDSPLowPass->remove();
		}
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetDistortion(bool bApply)
{
	//Default: 0.5
	this->SetDistortion(bApply, 0.5);
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetDistortion(bool bApply, float fLevel)
{
	bool bActive_;
	this->m_oDSPDistortion->getActive(&bActive_);

	if(bApply)
	{
		if(!bActive_)
		{
			this->m_oSys->addDSP(this->m_oDSPDistortion, 0);
		}
		
		fLevel = MIN(fLevel, 1.0);
		fLevel = MAX(fLevel, 0.0);
		
		this->m_oDSPDistortion->setParameter(FMOD_DSP_DISTORTION_LEVEL, fLevel);
	}
	else
	{
		if(bActive_)
		{
			this->m_oDSPDistortion->remove();
		}
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetPitchShift(bool bApply)
{
	//Default:2.0
	this->SetPitchShift(bApply, 2.0);
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetPitchShift(bool bApply, float fPitchScale)
{
	bool bActive_;
	this->m_oDSPPitchShift->getActive(&bActive_);

	if(bApply)
	{
		if(!bActive_)
		{
			this->m_oSys->addDSP(this->m_oDSPPitchShift, 0);
		}
		
		fPitchScale = MIN(fPitchScale, 2.0);
		fPitchScale = MAX(fPitchScale, 0.5);
		
		this->m_oDSPPitchShift->setParameter(FMOD_DSP_DISTORTION_LEVEL, fPitchScale);
	}
	else
	{
		if(bActive_)
		{
			this->m_oDSPPitchShift->remove();
		}
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetRoll(bool bApply)
{
	//default: eROLL_BEAT_ONE
	if(bApply)
	{
		this->SetRoll(bApply, eROLL_BEAT_ONE);
	}
	else
	{
		this->SetRoll(bApply, this->m_eNowRollType);
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetRoll(bool bApply, eROLL_TYPE eRollType)
{
	if(bApply)
	{
		if(eRollType != this->m_eNowRollType)
		{
			if(this->m_bIsRoll == false)
			{
				this->m_uint64RollTriggerSysTimeMS = ofGetElapsedTimeMillis();
			}
			this->m_uint64MicroTimer = ofGetElapsedTimeMicros();
			this->m_oChannel->getPosition(&this->m_uintRollTriggerSoundTimeMS, FMOD_TIMEUNIT_MS );
			
			if(this->m_bEnableCheatMode)
			{
				if(eRollType == this->m_stCheatList[this->m_iCheatIndex].eRollType)
				{
						this->m_uintRollStartTimeMS = this->m_stCheatList[this->m_iCheatIndex].uint64CheatTime;
						this->m_iCheatIndex++;
						this->m_iCheatIndex = this->m_iCheatIndex % 11;
				}
				else
				{
					if(( fmod(this->m_uintRollTriggerSoundTimeMS, this->m_fMSHalfBeat)) > this->m_fMSOneQuarteBeat)
					{
						this->m_uintRollStartTimeMS = (floor((float)this->m_uintRollTriggerSoundTimeMS / (float)this->m_fMSHalfBeat) + 1) * this->m_fMSHalfBeat;
					}
					else
					{
						this->m_uintRollStartTimeMS = floor((float)this->m_uintRollTriggerSoundTimeMS / (float)this->m_fMSHalfBeat) * this->m_fMSHalfBeat;
					}
				}
				
			}
			else
			{
				if(( fmod(this->m_uintRollTriggerSoundTimeMS, this->m_fMSHalfBeat)) > this->m_fMSOneQuarteBeat)
				{
					this->m_uintRollStartTimeMS = (floor((float)this->m_uintRollTriggerSoundTimeMS / (float)this->m_fMSHalfBeat) + 1) * this->m_fMSHalfBeat;
				}
				else
				{
					this->m_uintRollStartTimeMS = floor((float)this->m_uintRollTriggerSoundTimeMS / (float)this->m_fMSHalfBeat) * this->m_fMSHalfBeat;
				}
			}

			this->m_uint64RollMicroTime = this->GetRollMicroTime(eRollType);
			this->m_eNowRollType = eRollType;
			this->m_bIsRoll = bApply;
		}
	}
	else
	{
		if(eRollType == this->m_eNowRollType && this->m_bIsRoll == true)
		{
			unsigned long long uint64DiffMS_ = ofGetElapsedTimeMillis() - this->m_uint64RollTriggerSysTimeMS;
			this->m_oChannel->setPosition(this->m_uintRollTriggerSoundTimeMS + uint64DiffMS_, FMOD_TIMEUNIT_MS);
		
			this->m_uint64RollTriggerSysTimeMS = 0;
			this->m_uintRollTriggerSoundTimeMS = 0;
			this->m_uintRollStartTimeMS = 0;
			this->m_uint64RollMicroTime = 0;
			this->m_eNowRollType = eNONE_ROLL;

			this->m_bIsRoll = bApply;
		}
	}
}
//--------------------------------------------------------------
bool ofxEffectsSoundPlayer::IsHighPassActive()
{
	bool bIsActive_;
	this->m_oDSPHighPass->getActive(&bIsActive_);
	return bIsActive_;
}

//--------------------------------------------------------------
bool ofxEffectsSoundPlayer::IsDistortionActive()
{
	bool bIsActive_;
	this->m_oDSPDistortion->getActive(&bIsActive_);
	return bIsActive_;
}

//--------------------------------------------------------------
bool ofxEffectsSoundPlayer::IsPitchShiftActive()
{
	bool bIsActive_;
	this->m_oDSPPitchShift->getActive(&bIsActive_);
	return bIsActive_;
}

//--------------------------------------------------------------
bool ofxEffectsSoundPlayer::IsRollActive()
{
	return this->m_bIsRoll;
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetCheat(bool bEnableCheat)
{
	this->m_bEnableCheatMode = bEnableCheat;

	this->m_iCheatIndex = 0;
	if(bEnableCheat)
	{
		stROLL_CHEAT stRollCheat_;
		stRollCheat_.uint64CheatTime =  103.125 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[0] = stRollCheat_;

		stRollCheat_.uint64CheatTime =  106.875 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[1] = stRollCheat_;

		stRollCheat_.uint64CheatTime =  107.8125 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[2] = stRollCheat_;

		stRollCheat_.uint64CheatTime =  112.5 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[3] = stRollCheat_;

		stRollCheat_.uint64CheatTime =  113.4375 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[4] = stRollCheat_;

		stRollCheat_.uint64CheatTime = 114.375 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[5] = stRollCheat_;

		stRollCheat_.uint64CheatTime =  (114.375 + 0.702) * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[6] = stRollCheat_;

		stRollCheat_.uint64CheatTime =  (116.25 - 0.468) * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[7] = stRollCheat_;

		stRollCheat_.uint64CheatTime = 119.0625 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[8] = stRollCheat_;

		stRollCheat_.uint64CheatTime =   122.8125 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[9] = stRollCheat_;

		stRollCheat_.uint64CheatTime =   127.03125 * 1000;
		stRollCheat_.eRollType = eROLL_BEAT_ONE_HALF;
		this->m_stCheatList[10] = stRollCheat_;
	}
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::SetupRoll()
{
	this->m_oRollTimeSet[eROLL_BEAT_ONE_THIRTYTHREE] = (this->m_fMicroSPerBeat/32.0);
	this->m_oRollTimeSet[eROLL_BEAT_ONE_SIXTEEN] = ( this->m_fMicroSPerBeat/16.0);
	this->m_oRollTimeSet[eROLL_BEAT_ONE_EIGHT] = ( this->m_fMicroSPerBeat/8.0);
	this->m_oRollTimeSet[eROLL_BEAT_ONE_QUARTER] = ( this->m_fMicroSPerBeat/4.0);
	this->m_oRollTimeSet[eROLL_BEAT_ONE_HALF] = ( this->m_fMicroSPerBeat/2.0);
	this->m_oRollTimeSet[eROLL_BEAT_ONE] = this->m_fMicroSPerBeat;
	this->m_oRollTimeSet[eROLL_BEAT_TWO] = ( this->m_fMicroSPerBeat * 2.0);
}

//--------------------------------------------------------------
unsigned long long ofxEffectsSoundPlayer::GetRollMicroTime(eROLL_TYPE eRollType)
{	
	return unsigned long long(this->m_oRollTimeSet[eRollType] + 0.5);
}

//--------------------------------------------------------------
void ofxEffectsSoundPlayer::ErrorCheck()
{
	if(this->m_oResult != FMOD_OK)
	{
		std::cout<<FMOD_ErrorString(this->m_oResult);
	}
}

//--------------------------------------------------------------
//CLASS ofxEffectsAudioPlayer
//--------------------------------------------------------------
void ofxEffectsAudioPlayer::Setup()
{
	//Initial system
	this->m_oResult = FMOD::System_Create(&this->m_oSys);
	this->ErrorCheck();

	this->m_oSys->init(32, FMOD_INIT_NORMAL, 0);
	this->ErrorCheck();

	this->m_bIsSetup = true;

	this->ApplyReverb();
}
//--------------------------------------------------------------
void ofxEffectsAudioPlayer::Update()
{
	if(this->m_bIsSetup)
	{
		this->m_oSys->update();
	}
}
//--------------------------------------------------------------
void ofxEffectsAudioPlayer::Exit()
{
	EFFECTS_AUDIO_ITER oMapIter_ = this->m_oAudioMap.begin();
	for(;oMapIter_ != this->m_oAudioMap.end(); ++oMapIter_)
	{
		oMapIter_->second.pSound->release();
	}
	
	this->m_oSys->close();
	this->m_oSys->release();
}
//--------------------------------------------------------------
void ofxEffectsAudioPlayer::AddAudio(std::string strFilePath, std::string strID, bool bIsLoop)
{
	if(!this->m_bIsSetup)
	{
		this->Setup();
	}

	EFFECTS_AUDIO_ITER oIter_ = this->m_oAudioMap.find(strID);
	if(oIter_ == this->m_oAudioMap.end())
	{
		stEFFECTS_AUDIO stEffectsAudio_;

		this->m_oResult = this->m_oSys->createSound(strFilePath.c_str(), FMOD_SOFTWARE, 0, &stEffectsAudio_.pSound);
		this->ErrorCheck();

		if(bIsLoop)
		{
			stEffectsAudio_.pSound->setMode(FMOD_LOOP_NORMAL);
		}
		
		this->m_oAudioMap.insert(make_pair(strID, stEffectsAudio_));
	}
	else
	{
		ofLogWarning("[WARNING]This sound id is already exist:" + strID);
	}

}
//--------------------------------------------------------------
void ofxEffectsAudioPlayer::PlayAudio(std::string strID, float fVolume, unsigned int uiPosition)
{
	EFFECTS_AUDIO_ITER oIter_ = this->m_oAudioMap.find(strID);
	if(oIter_ != this->m_oAudioMap.end())
	{
		this->m_oSys->playSound(FMOD_CHANNEL_FREE, oIter_->second.pSound, false, &oIter_->second.pChannel);
		oIter_->second.pChannel->setVolume(fVolume);
		oIter_->second.pChannel->setPosition(uiPosition, FMOD_TIMEUNIT_MS);
	}
	else
	{
		ofLogWarning("[WARNING]Can't found sound id:" + strID);
	}
}

//--------------------------------------------------------------
void ofxEffectsAudioPlayer::StopAudio(std::string strID)
{
	bool bIsPlaying_ = false;

	EFFECTS_AUDIO_ITER oIter_ = this->m_oAudioMap.find(strID);
	if(oIter_ != this->m_oAudioMap.end())
	{
		oIter_->second.pChannel->isPlaying(&bIsPlaying_);

		if(bIsPlaying_)
		{
			oIter_->second.pChannel->stop();
		}
	}
	else
	{
		ofLogWarning("[WARNING]Can't found sound id:" + strID);
	}
}

//--------------------------------------------------------------
void ofxEffectsAudioPlayer::StopAllAudio()
{
	bool bIsPlaying_ = false;
	EFFECTS_AUDIO_ITER oIter_ = this->m_oAudioMap.begin();
	for(;oIter_ != this->m_oAudioMap.end(); ++oIter_)
	{
		oIter_->second.pChannel->isPlaying(&bIsPlaying_);

		if(bIsPlaying_)
		{
			oIter_->second.pChannel->stop();
		}
	}
}

//--------------------------------------------------------------
void ofxEffectsAudioPlayer::SetVolume(std::string strID, float fVolume)
{
	bool bIsPlaying_ = false;

	EFFECTS_AUDIO_ITER oIter_ = this->m_oAudioMap.find(strID);
	if(oIter_ != this->m_oAudioMap.end())
	{
		oIter_->second.pChannel->isPlaying(&bIsPlaying_);

		if(bIsPlaying_)
		{
			oIter_->second.pChannel->setVolume(fVolume);
		}
	}
	else
	{
		ofLogWarning("[WARNING]Can't found sound id:" + strID);
	}
}

//--------------------------------------------------------------
void ofxEffectsAudioPlayer::ApplyReverb()
{
	FMOD_REVERB_PROPERTIES prop_ = FMOD_PRESET_BATHROOM;
	this->m_oSys->setReverbProperties(&prop_);
}

//--------------------------------------------------------------
void ofxEffectsAudioPlayer::SetPosition(std::string strID, unsigned int uiPosition)
{
	bool bIsPlaying_ = false;

	EFFECTS_AUDIO_ITER oIter_ = this->m_oAudioMap.find(strID);
	if(oIter_ != this->m_oAudioMap.end())
	{
		oIter_->second.pChannel->isPlaying(&bIsPlaying_);

		if(bIsPlaying_)
		{
			oIter_->second.pChannel->setPosition(uiPosition, FMOD_TIMEUNIT_MS);
		}
	}
	else
	{
		ofLogWarning("[WARNING]Can't found sound id:" + strID);
	}
}

//--------------------------------------------------------------
void ofxEffectsAudioPlayer::ErrorCheck()
{
	if(this->m_oResult != FMOD_OK)
	{
		std::cout<<FMOD_ErrorString(this->m_oResult);
	}
}