#include "effectPlayer.h"

#pragma region Basic
//--------------------------------------------------------------
effectPlayer::effectPlayer()
	:_vol(0.0)
	,_eState(eStateMute)
{}

//--------------------------------------------------------------
effectPlayer::~effectPlayer()
{
}
#pragma endregion

#pragma region Control
//--------------------------------------------------------------
bool effectPlayer::load(string path, bool loop)
{
	if (!_player.loadSound(path))
	{
		ofLog(OF_LOG_ERROR, "[effectPlayer]load audio failed : " + path);
		return false;
	}

	_player.setLoop(loop);
	_player.setMultiPlay(false);
	return true;
}

//--------------------------------------------------------------
void effectPlayer::play()
{
	if (_player.isLoaded() && !_player.getIsPlaying())
	{
		_vol = 0.0;
		_player.setVolume(_vol);
		_player.play();
		_eState = eStateMute;
	}
}

//--------------------------------------------------------------
void effectPlayer::stop()
{
	if (_player.getIsPlaying())
	{
		_player.stop();
	}
}

//--------------------------------------------------------------
void effectPlayer::in()
{
	if (_player.getIsPlaying())
	{
		_vol = 1.0;
		_player.setVolume(_vol);
		_eState = eStatePlay;
	}
}

//--------------------------------------------------------------
void effectPlayer::out()
{
	if (_player.getIsPlaying())
	{
		_vol = 0.0;
		_player.setVolume(_vol);
		_eState = eStateMute;
	}
}

//--------------------------------------------------------------
eEffectFadeState effectPlayer::getState() const
{
	return _eState;
}
#pragma endregion

