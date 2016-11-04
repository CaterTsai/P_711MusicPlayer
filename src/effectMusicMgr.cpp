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
	updateAllPlayer(delta);
	updateEffect(delta);
}

//--------------------------------------------------------------
bool effectMusicMgr::isPlaying()
{
	return _isEffectMusicStart;
}

#pragma region Basic Music
//--------------------------------------------------------------
void effectMusicMgr::loadBasic(string path)
{
	_basicPlayer.loadSound(path);
	_basicPlayer.setLoop(true);
}

//--------------------------------------------------------------
void effectMusicMgr::playBasic()
{
	_basicPlayer.play();
}

//--------------------------------------------------------------
void effectMusicMgr::stopBasic()
{
	_basicPlayer.stop();
}
#pragma endregion

#pragma region Effect Player
//-------------------
//EffectPlayer
//-------------------
void effectMusicMgr::addPlayer(eAudioType eType, eAudioGroup eGroup, ePlayerType ePType, string path)
{
	if (_playerMgr.find(eType) == _playerMgr.end())
	{
		switch (ePType)
		{
			case eLoopingPlayer:
			{
				_playerMgr.insert(make_pair(eType, ofPtr<loopingPlayer>(new loopingPlayer())));
				break;
			}
			case eTriggerPlayer:
			{
				_playerMgr.insert(make_pair(eType, ofPtr<triggerPlayer>(new triggerPlayer())));
				break;
			}
		}
		
		_playerMgr[eType]->load(path);

		addToGroupSet(eType, eGroup);
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[effectMusic]This audio type already exist");
	}
}

//--------------------------------------------------------------
void effectMusicMgr::play()
{
	_isEffectMusicStart = true;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->play();
	}
}

//--------------------------------------------------------------
void effectMusicMgr::stop()
{
	_isEffectMusicStart = false;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->stop();
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
	if (aGroup == eAudioGroup::eAudioGroup_Left)
	{
		_groupLeftVol = vol;
		updateGroupVol(eAudioGroup::eAudioGroup_Left);
	}
	else if(aGroup == eAudioGroup::eAudioGroup_Right)
	{
		_groupRightVol = vol;
		updateGroupVol(eAudioGroup::eAudioGroup_Right);
	}
}

//--------------------------------------------------------------
void effectMusicMgr::updateAllPlayer(float fDelta)
{
	for (auto& Iter_ : _playerMgr)
	{
		//Iter_.second.update(fDelta);
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
	ofPtr<effectDSP> echo_ = ofPtr<effectDSP>(new echoDSP());
	echo_->setup(FMOD_DSP_TYPE_ECHO);

	ofPtr<effectDSP> distortion_ = ofPtr<effectDSP>(new distortionDSP());
	distortion_->setup(FMOD_DSP_TYPE_DISTORTION);

	ofPtr<effectDSP> lowpass_ = ofPtr<effectDSP>(new lowpassDSP());
	lowpass_->setup(FMOD_DSP_TYPE_LOWPASS);

	_effectMgr.insert(make_pair(eEffect_Echo, echo_));
	_effectMgr.insert(make_pair(eEffect_Distortion, distortion_));
	_effectMgr.insert(make_pair(eEffect_LowPass, lowpass_));

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