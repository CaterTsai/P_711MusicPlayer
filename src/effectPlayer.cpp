#include "effectPlayer.h"

#pragma region basicPlayer
//--------------------------------------------------------------
bool basicPlayer::load(string path, bool loop)
{
	if (!_player.loadSound(path))
	{
		ofLog(OF_LOG_ERROR, "[basicPlayer]load audio failed : " + path);
		return false;
	}

	_player.setLoop(loop);
	_player.setMultiPlay(false);
	return true;
}
#pragma endregion

#pragma region loopingPlayer
#pragma region Basic
//--------------------------------------------------------------
loopingPlayer::loopingPlayer(float extendLength)
	:basicPlayer()
	,_eState(eStateWait)
	,_vol(1.0f)
	,_extendLength(extendLength)
{
}

//--------------------------------------------------------------
loopingPlayer::~loopingPlayer()
{
}
#pragma endregion

#pragma region Control
//--------------------------------------------------------------
void loopingPlayer::update(float delta)
{
	if (_eState == eStateExtend)
	{
		_timer -= delta;
		if (_timer <= 0.0f)
		{
			_vol = 0.0;
			_player.setVolume(_vol);
			_eState = eStateWait;
		}
	}
}

//--------------------------------------------------------------
void loopingPlayer::play()
{
	if (_player.isLoaded() && !_player.getIsPlaying())
	{
		_vol = 0.0;
		_player.setVolume(_vol);
		_player.play();
		_eState = eStateWait;
	}
}

//--------------------------------------------------------------
void loopingPlayer::stop()
{
	if (_player.getIsPlaying())
	{
		_player.stop();
	}
}

//--------------------------------------------------------------
void loopingPlayer::in()
{
	if (_player.getIsPlaying())
	{
		_vol = _maxVol;
		_player.setVolume(_vol);
		_eState = eStatePlay;
	}
}

//--------------------------------------------------------------
void loopingPlayer::out()
{
	if (_player.getIsPlaying())
	{
		if (_eState == eStatePlay)
		{
			_eState = eStateExtend;
			_timer = _extendLength;
		}
	}
}

//--------------------------------------------------------------
bool loopingPlayer::isPlaying()
{
	return (_eState == eStatePlay);
}

//--------------------------------------------------------------
float loopingPlayer::getMaxVol()
{
	return _maxVol;
}

//--------------------------------------------------------------
eFadeState loopingPlayer::getState() const
{
	return _eState;
}
#pragma endregion
#pragma endregion

#pragma region triggerPlayer
#pragma region Basic
//--------------------------------------------------------------
triggerPlayer::triggerPlayer()
	:basicPlayer()
	,_isStart(false)
{
}

//--------------------------------------------------------------
triggerPlayer::~triggerPlayer()
{
}
#pragma endregion

#pragma region Control
//--------------------------------------------------------------
void triggerPlayer::play()
{
	_isStart = true;
}

//--------------------------------------------------------------
void triggerPlayer::stop()
{
	if (_player.getIsPlaying())
	{
		_player.stop();
	}
	_isStart = false;
}

//--------------------------------------------------------------
void triggerPlayer::in()
{
	if (_isStart)
	{
		_player.setVolume(_maxVol);
		_player.play();
	}
}

//--------------------------------------------------------------
void triggerPlayer::out()
{
	//if (_isStart)
	//{
	//	_player.stop();
	//}
}

//--------------------------------------------------------------
bool triggerPlayer::isPlaying()
{
	return _player.getIsPlaying();
}
#pragma endregion


#pragma endregion
