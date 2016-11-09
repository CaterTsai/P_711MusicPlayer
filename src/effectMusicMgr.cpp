#include "effectMusicMgr.h"

//--------------------------------------------------------------
void effectMusicMgr::setup()
{
	setupEffect();

	_groupBlance = 0.5f;
	_groupLeftVol = _groupRightVol = 1.0f;
}

//--------------------------------------------------------------
void effectMusicMgr::update(float delta)
{
	_animMainVol.update(delta);

	updateAllPlayer(delta);
	updateEffect(delta);

	if (_animMainVol.isAnimating())
	{
		setGroupVol(eAudioGroup::eAudioGroup_Left, _animMainVol.getCurrentValue());
		setGroupVol(eAudioGroup::eAudioGroup_Right, _animMainVol.getCurrentValue());
		setGroupVol(eAudioGroup::eAudioGroup_Button, _animMainVol.getCurrentValue());
		setBasicVol(_animMainVol.getCurrentValue());
		//ofSoundSetVolume(_animMainVol.getCurrentValue());
	}
}

//--------------------------------------------------------------
bool effectMusicMgr::isPlaying()
{
	return _isEffectMusicStart;
}

//--------------------------------------------------------------
void effectMusicMgr::play()
{
	_animMainVol.setDuration(cCoinFadeinTime);
	_animMainVol.animateFromTo(0.0, 1.0);

	_isEffectMusicStart = true;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->play();
	}

	if (_basicPlayer.isLoaded())
	{
		_basicPlayer.play();
	}
	//ofSoundSetVolume(0.0f);

}

//--------------------------------------------------------------
void effectMusicMgr::stop()
{
	_isEffectMusicStart = false;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->stop();
	}
	_basicPlayer.stop();
}

//--------------------------------------------------------------
void effectMusicMgr::fadeout()
{
	_animMainVol.setDuration(cCoinFadeinTime);
	_animMainVol.animateFromTo(1.0, 0.0);
}

#pragma region Basic Music
//--------------------------------------------------------------
void effectMusicMgr::loadBasic(string path)
{
	_basicPlayer.loadSound(path);
	_basicPlayer.setLoop(true);
}

//--------------------------------------------------------------
void effectMusicMgr::setBasicVol(float vol)
{
	_basicPlayer.setVolume(vol);
}
#pragma endregion

#pragma region Effect Player
//-------------------
//EffectPlayer
//-------------------
void effectMusicMgr::addPlayer(eAudioType eType, eAudioGroup eGroup, ePlayerType ePType, string path, float extendTime)
{
	if (_playerMgr.find(eType) == _playerMgr.end())
	{
		switch (ePType)
		{
			case eLoopingPlayer:
			{
				_playerMgr.insert(make_pair(eType, ofPtr<loopingPlayer>(new loopingPlayer(extendTime))));
				_playerMgr[eType]->load(path);
				break;
			}
			case eTriggerPlayer:
			{
				_playerMgr.insert(make_pair(eType, ofPtr<triggerPlayer>(new triggerPlayer())));
				_playerMgr[eType]->load(path, false);
				break;
			}
		}
		addToGroupSet(eType, eGroup);
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[effectMusic]This audio type already exist");
	}
}

//-------------------------------------------------------------
bool effectMusicMgr::getIsPlaying(eAudioType eType)
{
	try {
		auto& Iter_ = _playerMgr.at(eType);
		return Iter_->isPlaying();
	}
	catch (out_of_range e) {
		ofLog(OF_LOG_ERROR, "[effectMusicMgr:getPlayerState]Out of range");
		return eStateUnknow;
	}
}

//--------------------------------------------------------------
void effectMusicMgr::playerToggle(eAudioType eType)
{
	if (!_isEffectMusicStart)
	{
		return;
	}
	bool isPlaying_ = getIsPlaying(eType);
	if (!isPlaying_)
	{
		playerIn(eType);
	}
	else if (isPlaying_)
	{
		playerOut(eType);
	}
}

//--------------------------------------------------------------
void effectMusicMgr::playerIn(eAudioType eType)
{
	if (!_isEffectMusicStart)
	{
		return;
	}
	try {
		auto& Iter_ = _playerMgr.at(eType);
		Iter_->in();
	}
	catch (out_of_range e){
		ofLog(OF_LOG_ERROR, "[effectMusicMgr:playerIn]Out of range");
	}	
}

//--------------------------------------------------------------
void effectMusicMgr::playerOut(eAudioType eType)
{
	if (!_isEffectMusicStart)
	{
		return;
	}
	try {
		auto& Iter_ = _playerMgr.at(eType);
		Iter_->out();
	}
	catch (out_of_range e) {
		ofLog(OF_LOG_ERROR, "[effectMusicMgr:playerOut]Out of range");
	}
}

//--------------------------------------------------------------
void effectMusicMgr::setGroupBalance(float val)
{
	_groupBlance = val;

	updateGroupVol(eAudioGroup::eAudioGroup_Left);
	updateGroupVol(eAudioGroup::eAudioGroup_Right);
}

//--------------------------------------------------------------
void effectMusicMgr::setGroupVol(eAudioGroup aGroup, float vol)
{
	auto audioList_ = _groupSet[aGroup];
	for (auto& iter_ : audioList_)
	{
		_playerMgr[iter_]->setMaxVol(vol);
	}
}

//--------------------------------------------------------------
void effectMusicMgr::updateAllPlayer(float fDelta)
{
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->update(fDelta);
	}
}

//--------------------------------------------------------------
void effectMusicMgr::addToGroupSet(eAudioType eType, eAudioGroup eGroup)
{
	if (_groupSet.find(eGroup) != _groupSet.end())
	{
		_groupSet[eGroup].push_back(eType);
	}
	else
	{
		vector<eAudioType> groupList_;
		groupList_.push_back(eType);
		_groupSet.insert(make_pair(eGroup, groupList_));
	}
}

//--------------------------------------------------------------
void effectMusicMgr::updateGroupVol(eAudioGroup eGroup)
{
	auto audioList_ = _groupSet[eGroup];
	float sourceVol_, Val_;
	if (eGroup == eAudioGroup::eAudioGroup_Left)
	{
		sourceVol_ = _groupLeftVol;
		Val_ = MIN( (1.0f - _groupBlance)/0.5f, 1.0f);
	}
	else if (eGroup == eAudioGroup::eAudioGroup_Right)
	{
		sourceVol_ = _groupRightVol;
		Val_ = MIN(_groupBlance / 0.5f, 1.0f);
	}

	if (Val_ < 1.0f)
	{
		sourceVol_ *= Val_;
	}
	
	for (auto& iter_ : audioList_)
	{
		_playerMgr[iter_]->setMaxVol(sourceVol_);
	}
}


#pragma endregion

#pragma region Effect
//--------------------------------------------------------------
void effectMusicMgr::setupEffect()
{

	ofPtr<effectDSP> distortion_ = ofPtr<effectDSP>(new distortionDSP());
	distortion_->setup(FMOD_DSP_TYPE_DISTORTION);

	ofPtr<effectDSP> pitchshift = ofPtr<effectDSP>(new pitchShifterDSP());
	pitchshift->setup(FMOD_DSP_TYPE_PITCHSHIFT);

	_effectMgr.insert(make_pair(eEffect_Distortion, distortion_));
	_effectMgr.insert(make_pair(eEffect_PitchShift, pitchshift));

	//FMOD::Reverb3D
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_AUDITORIUM;
	ofFmodGetSystem()->setReverbProperties(0, &prop);
}

//--------------------------------------------------------------
void effectMusicMgr::updateEffect(float delta)
{
	for (auto& Iter_ : _effectMgr)
	{
		Iter_.second->update(delta);
	}
}

//--------------------------------------------------------------
void effectMusicMgr::activeEffect(eEffectType eType)
{
	if (!_isEffectMusicStart)
	{
		return;
	}

	try
	{
		auto Iter_ = _effectMgr.at(eType);
		Iter_->setActive(true);
	}
	catch (out_of_range e)
	{
		ofLog(OF_LOG_ERROR, "[effectMusicMgr-activeEffect]Out of range");
	}
}

//--------------------------------------------------------------
void effectMusicMgr::unactiveEffect(eEffectType eType)
{
	if (!_isEffectMusicStart)
	{
		return;
	}

	try
	{
		auto Iter_ = _effectMgr.at(eType);
		Iter_->setActive(false);
	}
	catch (out_of_range e)
	{
		ofLog(OF_LOG_ERROR, "[effectMusicMgr-unactiveEffect]Out of range");
	}
}

//--------------------------------------------------------------
void effectMusicMgr::troggleEffect(eEffectType eType)
{
	if (!_isEffectMusicStart)
	{
		return;
	}

	try
	{
		auto Iter_ = _effectMgr.at(eType);
		Iter_->troggle();
	}
	catch (out_of_range e)
	{
		ofLog(OF_LOG_ERROR, "[effectMusicMgr-troggleEffect]Out of range");
	}
}
#pragma endregion

#pragma region Singletion
//--------------------------------------------------------------
effectMusicMgr* effectMusicMgr::pInstance = 0;

//--------------------------------------------------------------
effectMusicMgr* effectMusicMgr::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new effectMusicMgr();
	}
	return pInstance;
}

//--------------------------------------------------------------
void effectMusicMgr::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion