#pragma once

#include "constParameter.h"
#include "ofxXmlSettings.h"

class config
{
public:
	void load(string path);

public:
	string audioPath;
	string videoPath;
	string devicePort;
	string sensorLeftPort, sensorRightPort;



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
