#pragma once

#include "constParameter.h"

class idleVoice
{
public:
	idleVoice()
		:_playID(-1)
		, _playing(false)
		, _waitTime(6000)
		, _startHour(8)
		, _endHour(22)
	{}
	void addVoice(string strPath);
	void update(float delta);

	void start();
	void stop();
	
	void setWaitTime(float waitTime);
	void setRange(int startHour, int endHour);
private:
	bool _playing;
	int _startHour, _endHour;
	float _waitTime, _timer;
	vector<ofSoundPlayer>	_playList;
	int _playID;
};