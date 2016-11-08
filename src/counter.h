#pragma once

#include "ofMain.h"

class counterEvent
{
public:
	counterEvent(int time, string eventMsg) 
		:_time(time)
		, _eventMsg(eventMsg)
	{}

	//------------------------
	bool check(float time)
	{
		if (_isCheck)
		{
			return false;
		}

		if (time <= _time)
		{
			_isCheck = true;
			return true;
		}
		return false;
	}

	//------------------------
	void reset()
	{
		_isCheck = false;
	}

	//------------------------
	string getMsg()
	{
		return _eventMsg;
	}
private:
	bool _isCheck;
	float _time;
	string _eventMsg;
};

class counter
{
public:
	counter()
		:_isSetup(false)
		, _isStart(false)
		, _numWidth(0)
		, _numHeight(0)
		, _counterTimer(0)
		, _targetTime(0)
	{}
	void setup(const string numberfolder);
	void update(const float delta);
	void draw(ofVec2f pos);

	void start(float time);
	void stop();

private:
	bool _isSetup, _isStart;
	float _counterTimer,_targetTime;

	vector<ofImage> _numImgList;
	float _numWidth, _numHeight;

//-------------------------
//Counter Event
//-------------------------
public:
	void addCounterEvent(float time, string msg);

private:
	void updateCounterEvent();
	void resetCounterEvent();
private:
	vector<counterEvent> _counterEvent;

//-------------------------
//EVENT
public:
	static ofEvent<string> _countEvent;

};