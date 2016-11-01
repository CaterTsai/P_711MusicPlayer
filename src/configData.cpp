#include "configData.h"


//--------------------------------------------------------------
config* config::pInstance = 0;

void config::load(string path)
{
	ofxXmlSettings _config;
	if (!_config.loadFile(path))
	{
		ofLog(OF_LOG_ERROR, "[config::load]load config failed");
		return;
	}

	audioPath = _config.getValue("audioPath", "audiosTPE/", 0);
	videoPath = _config.getValue("videoPath", "videosTPE/", 0);
	devicePort = _config.getValue("devicePort", "COM3", 0);
	sensorLeftPort = _config.getValue("sensorLeftPort", "COM5", 0);
	sensorRightPort = _config.getValue("sensorRightPort", "COM6", 0);
}

//--------------------------------------------------------------
config::config()
	:audioPath("audiosTPE/")
	,videoPath("videosTPE/")
	, devicePort("COM3")
	, sensorLeftPort("COM5")
	, sensorRightPort("COM6")
{}

//--------------------------------------------------------------
config* config::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new config();
	}
	return pInstance;
}

//--------------------------------------------------------------
void config::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}