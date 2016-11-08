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

	_config.pushTag("channelInfo");
		audioPath = _config.getValue("audioPath", "audiosTPE/", 0);
		videoPath = _config.getValue("videoPath", "videosTPE/", 0);

		int channelNum_ = _config.getNumTags("channel");
		channelData.resize(channelNum_);
		for (int idx_ = 0; idx_ < channelNum_; idx_++)
		{
			_config.pushTag("channel", idx_);
			channelData[idx_]._type = (eAudioType)_config.getValue("id", 0, 0);
			channelData[idx_]._playerType = (ePlayerType)_config.getValue("playertype", 0, 0);
			channelData[idx_]._audioGroup = (eAudioGroup)_config.getValue("audiogroup", 0, 0);
			channelData[idx_]._name = _config.getValue("name", "", 0);
			channelData[idx_]._level = _config.getValue("level", 0, 0);
			channelData[idx_]._extendTime = _config.getValue("extendTime", 0.0f, 0);


			_config.popTag();
		}
	_config.popTag();

	
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