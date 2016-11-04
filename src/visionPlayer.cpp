#include "visionPlayer.h"

#pragma region basicVisionPlayer
//--------------------------------------------------------------
basicVisionPlayer::basicVisionPlayer(int drawLevel)
	:_drawLevel(drawLevel)
{}

//--------------------------------------------------------------
basicVisionPlayer::~basicVisionPlayer()
{
	_player.close();
}

//--------------------------------------------------------------
bool basicVisionPlayer::load(string path)
{
	_player.setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer()));
	_player.setLoopState(ofLoopType::OF_LOOP_NORMAL);
	if (!_player.loadMovie(path))
	{
		ofLog(OF_LOG_ERROR, "[loopingVisionPlayer]load video failed : " + path);
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void basicVisionPlayer::update(float delta)
{
	if (_player.isLoaded())
	{
		_player.update();
	}
}

//--------------------------------------------------------------
void basicVisionPlayer::draw(int x, int y, int width, int height)
{
	if (!_player.isLoaded())
	{
		return;
	}

	if (_alpha > 0 && _player.isPlaying())
	{
		ofPushStyle();
		ofSetColor(255, _alpha);
		ofPushMatrix();
		ofTranslate(x, y);
		{
			_player.draw(width * -0.5, height * -0.5, width, height);
		}
		ofPopMatrix();
		ofPopStyle();
	}
}

//--------------------------------------------------------------
int basicVisionPlayer::getDrawLevel() const
{
	return _drawLevel;
}
#pragma endregion

#pragma region loopingVisionPlayer
//--------------------------------------------------------------
loopingVisionPlayer::loopingVisionPlayer(int drawLevel)
	:_eState(eStateWait)
	,basicVisionPlayer(drawLevel)
{
}

//--------------------------------------------------------------
loopingVisionPlayer::~loopingVisionPlayer()
{
	_player.close();
}

//--------------------------------------------------------------
void loopingVisionPlayer::play()
{
	if (_player.isLoaded())
	{
		_player.play();
		_eState = eStateWait;
	}
}

//--------------------------------------------------------------
void loopingVisionPlayer::stop()
{
	if (_player.isPlaying())
	{
		_player.stop();
	}
}

//--------------------------------------------------------------
void loopingVisionPlayer::in()
{
	if (_player.isPlaying())
	{
		_alpha = 255.0;
		_eState = eStatePlay;
	}
}

//--------------------------------------------------------------
void loopingVisionPlayer::out()
{
	if (_player.isPlaying())
	{
		_alpha = 0.0;
		_eState = eStateWait;
	}
}

//--------------------------------------------------------------
bool loopingVisionPlayer::isPlaying()
{
	return (_eState == eStatePlay);
}

//--------------------------------------------------------------
eFadeState loopingVisionPlayer::getState() const
{
	return _eState;
}
#pragma endregion

#pragma region triggerVisionPlayer
triggerVisionPlayer::triggerVisionPlayer(int drawLevel)
	:basicVisionPlayer(drawLevel)
	,_isStart(false)
{

}
//--------------------------------------------------------------
void triggerVisionPlayer::play()
{
	_isStart = true;
}

//--------------------------------------------------------------
void triggerVisionPlayer::stop()
{
	if (_player.isPlaying())
	{
		_player.stop();
	}
	_isStart = true;
}

//--------------------------------------------------------------
void triggerVisionPlayer::in()
{	
	_player.play();
	_alpha = 255.0f;
}

//--------------------------------------------------------------
void triggerVisionPlayer::out()
{
	_player.stop();
	_player.setFrame(0);
	_player.update();
	_alpha = 0.0f;
}

//--------------------------------------------------------------
bool triggerVisionPlayer::isPlaying()
{
	return _player.isPlaying();
}
#pragma endregion


