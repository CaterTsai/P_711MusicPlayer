#include "effectMusicMgr.h"

//--------------------------------------------------------------
void effectMusicMgr::setup()
{
	setupEffect();
}

//--------------------------------------------------------------
void effectMusicMgr::update(float delta)
{
	updateAllPlayer(delta);
	updateEffect(delta);
}

bool effectMusicMgr::isPlaying()
{
	return _isPlaying;
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
void effectMusicMgr::addPlayer(eAudioType eType, string path)
{
	if (_playerMgr.find(eType) == _playerMgr.end())
	{
		effectPlayer newPlayer_;
		_playerMgr.insert(make_pair(eType, newPlayer_));
		_playerMgr[eType].load(path);
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[effectMusic]This audio type already exist");
	}
}

//--------------------------------------------------------------
void effectMusicMgr::play()
{
	_isPlaying = true;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second.play();
	}
}

//--------------------------------------------------------------
void effectMusicMgr::stop()
{
	_isPlaying = false;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second.stop();
	}
}

//-------------------------------------------------------------
eEffectFadeState effectMusicMgr::getPlayerState(eAudioType eType)
{
	try {
		auto& Iter_ = _playerMgr.at(eType);
		return Iter_.getState();
	}
	catch (out_of_range e) {
		ofLog(OF_LOG_ERROR, "[effectMusicMgr:getPlayerState]Out of range");
		return eStateUnknow;
	}
}

//--------------------------------------------------------------
void effectMusicMgr::playerIn(eAudioType eType)
{
	try {
		auto& Iter_ = _playerMgr.at(eType);
		Iter_.in();
	}
	catch (out_of_range e){
		ofLog(OF_LOG_ERROR, "[effectMusicMgr:playerIn]Out of range");
	}	
}

//--------------------------------------------------------------
void effectMusicMgr::playerOut(eAudioType eType)
{
	try {
		auto& Iter_ = _playerMgr.at(eType);
		Iter_.out();
	}
	catch (out_of_range e) {
		ofLog(OF_LOG_ERROR, "[effectMusicMgr:playerOut]Out of range");
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


#pragma endregion

#pragma region Effect
//--------------------------------------------------------------
void effectMusicMgr::setupEffect()
{
	ofPtr<effectDSP> echo_ = ofPtr<effectDSP>(new echoDSP());
	echo_->setup(FMOD_DSP_TYPE_ECHO);

	ofPtr<effectDSP> distortion_ = ofPtr<effectDSP>(new distortionDSP());
	distortion_->setup(FMOD_DSP_TYPE_DISTORTION);

	ofPtr<effectDSP> pitchshift_ = ofPtr<effectDSP>(new pitchShiftDSP());
	pitchshift_->setup(FMOD_DSP_TYPE_PITCHSHIFT);

	_effectMgr.insert(make_pair(eEffect_Echo, echo_));
	_effectMgr.insert(make_pair(eEffect_Distortion, distortion_));
	_effectMgr.insert(make_pair(eEffect_PitchShift, pitchshift_));
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