#pragma once

#include "constParameter.h"
#include "ofxXmlSettings.h"

class channelData
{
public:
	eAudioType _type;
	ePlayerType _playerType;
	eAudioGroup	_audioGroup;
	float _extendTime;
	string _name;
	int _level;
};

class config
{
public:
	void load(string path);

public:
	string audioPath;
	string videoPath;
	string devicePort;
	string sensorLeftPort, sensorRightPort;
	
	vector<channelData> channelData;


private:
	config();
	~config(){}
	config(config const&) {};
	void operator=(config const&) {};

public:
	static config* GetInstance();
	static void Destroy();

private:
	static config *pInstance;
};
