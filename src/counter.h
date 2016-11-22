#pragma once

#include "constParameter.h"

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

		if (time >= _time)
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
		:_isStart(false)
		, _counterTimer(0)
		, _targetTime(0)
	{}

	void load(string iconPath, string fontPath);
	void update(const float delta);
	void draw(ofRectangle rect);

	void start(float time);
	void stop();

private:
	bool _isStart;
	float _counterTimer,_targetTime;
	ofTrueTypeFont	_font;
	ofImage _heart;

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