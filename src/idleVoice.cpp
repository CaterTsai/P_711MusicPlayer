#include "idleVoice.h"

//--------------------------------------------------------------
void idleVoice::addVoice(string strPath)
{
	ofSoundPlayer newPlayer_;
	if (!newPlayer_.loadSound(strPath))
	{
		ofLog(OF_LOG_WARNING, "[idleVoice::addBGM]load bgm failed :" + strPath);
		return;
	}
	newPlayer_.setLoop(false);
	_playList.push_back(newPlayer_);

}

//--------------------------------------------------------------
void idleVoice::update(float delta)
{
	if (_playList.size() <= 0 || !_playing)
	{
		return;
	}
	
	_timer -= delta;
	if (_timer <= 0.0f)
	{
		try
		{
			_playList.at(_playID).play();
			_timer = _waitTime;
			_playID = (_playID + 1) % _playList.size();
		}
		catch (exception& e)
		{
			ofLog(OF_LOG_ERROR, e.what());
		}
		
	}

}

//--------------------------------------------------------------
void idleVoice::start()
{	
	if (_playList.size() <= 0)
	{
		return;
	}

	int hour_ = stoi(ofGetTimestampString("%H"));
	if (hour_ < _startHour || hour_ >= _endHour)
	{
		return;
	}

	try {
		_playID = 0;
		_playing = true;
		_timer = _waitTime;
	}
	catch (exception e)
	{
		ofLog(OF_LOG_ERROR, e.what());
	}

}

//--------------------------------------------------------------
void idleVoice::stop()
{
	
	if (_playList.size() <= 0)
	{
		return;
	}

	try {
		if (_playList.at(_playID).getIsPlaying())
		{
			_playList.at(_playID).stop();
		}
	}
	catch (exception e)
	{
		ofLog(OF_LOG_ERROR, e.what());
	}
	
	_playID = 0;
	_playing = false;
}

//--------------------------------------------------------------
void idleVoice::setWaitTime(float waitTime)
{
	_waitTime = waitTime;
}

//--------------------------------------------------------------
void idleVoice::setRange(int startHour, int endHour)
{
	_startHour = startHour;
	_endHour = endHour;
}
