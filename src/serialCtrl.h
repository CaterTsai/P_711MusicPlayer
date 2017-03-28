#pragma once

#include "constParameter.h"

template <class T>
class serialArgs{

public:
	serialArgs(unsigned char type, unsigned char no, T val)
		:_type(type)
		,_no(no)
		,_param(val)
	{}

	unsigned char _type, _no;
	T _param;	
};

//------------------------------------
class serialCtrl
{
public:
	serialCtrl()
		:_isSetup(false)
		, _msgComplete(false)
	{}
	void setup(string port, int baud);
	void update();
	void send(unsigned char val);

protected:
	void read();
	void sendVal();
	virtual void decodeMsg();

protected:
	bool _isSetup, _msgComplete;
	string _msg;
	ofSerial _serial;

	bool _needSend;
	unsigned char _val;

};

//------------------------------------
class deviceCtrl : public serialCtrl
{
public:
	deviceCtrl()
		:serialCtrl()
	{
		_isOpen = false;
		ZeroMemory(_isTrigger, sizeof(bool) * cBtnNum);
	}

	void open();
	void close();
	bool isOpen();

private:
	void decodeMsg() override;

private:
	void buttonEvent();
	void sliderEvent();

private:
	bool _isOpen;
	bool _isTrigger[cBtnNum];

//----------------
//event
public:
	ofEvent<serialArgs<bool>>	_buttonEvent;
	ofEvent<serialArgs<float>>  _sliderEvent;

};

//------------------------------------
class sensorCtrl : public serialCtrl
{
public:
	sensorCtrl()
		:serialCtrl()
	{
		ZeroMemory(_isTrigger, sizeof(bool)*cCoinSetNum*cCoinNumEachSet);
	}

	void reset();
private:

	void decodeMsg() override;
	void sensorEvent();

private:
	bool _isTrigger[cCoinSetNum * cCoinNumEachSet];
//----------------
//event
public:
	ofEvent<serialArgs<bool>>	_sensorEvent;
};