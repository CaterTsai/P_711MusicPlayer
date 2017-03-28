#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	_pauseSensor = false;
	_performanceMode = false;
	//ofLogToFile("Log.txt", true);
#ifndef _DEBUG
	ofToggleFullscreen();
	ofHideCursor();
	_hideCursor = true;
	
#else
	_hideCursor = false;
#endif // _DEBUG

	config::GetInstance()->load("_config.xml");
	loadAllMusic();
	loadAllVideo();

	//Counter
	setupCounter();

	//VideoMgr
	setupVideoMgr();

	//Arduino
	setupArduino();

	//System Caller
	_caller.start();
	ofAddListener(_caller.SystemCallerFinishEvent, this, &ofApp::onSystemCallFinish);

	//Uploader
	ofAddListener(_httpConn.newResponseEvent, this, &ofApp::httpRespone);
	_httpConn.start();

	//QR Printer
	_qrPrinter.setup();

	//Idle Voice
	setupIdleVoice();

#ifdef _USE_SIMULATION_
	setupSimulation();
#endif // _USE_SIMULATION_

	ofLog(OF_LOG_NOTICE, "[ofApp::Setup] Finish");

	ofBackground(0);
	ofSetVerticalSync(true);

	_videoMgr.play(eVideoType::eVideoTutorial);
	_idleVoice.start();
	_mainTimer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	float delta_ = ofGetElapsedTimef() - _mainTimer;
	_mainTimer += delta_;

	effectMusicMgr::GetInstance()->update(delta_);
	visionDisplay::GetInstance()->update(delta_);

	_ctrlDevice.update();
	_coinSensorLeft.update();
	_coinSensorRight.update();

	_videoMgr.update(delta_);
	updateCounter(delta_);
	updateArduino(delta_);

	_idleVoice.update(delta_);

#ifdef _USE_SIMULATION_
	updateSimulation(delta_);
#endif // _USE_SIMULATION_

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef _USE_SIMULATION_
	drawSimulation();
#endif // _USE_SIMULATION_


	visionDisplay::GetInstance()->draw();	
	_videoMgr.draw();

	drawCounter();
	drawMessage();


}

//--------------------------------------------------------------
void ofApp::exit()
{
	simulatorKey::GetInstance()->sendKey(cEndRocodeKey);
	_caller.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
		case 'a':
		{
			startCoin();
			break;
		}
		case 's':
		{
			stopCoin();
			break;
		}
		case 'z':
		{
			startPerformance();
			break;
		}
		case 'x':
		{
			stopPerformance();
			break;
		}
		case 'd':
		{
			_muteBasic = !_muteBasic;
			_muteBasic ? effectMusicMgr::GetInstance()->setBasicVol(0.0f) : effectMusicMgr::GetInstance()->setBasicVol(1.0f);
			break;
		}

		//Fullscreen
		case 'f':
		{
			ofToggleFullscreen();
			break;
		}

		case 'g':
		{
			_hideCursor = !_hideCursor;
			_hideCursor ? ofHideCursor() : ofShowCursor();
			break;
		}

		//Ctrl
		case '1':
		{
			_motorCtrl ^= true;
			break;
		}
		case '2':
		{
			_performanceMode ^= true;
			break;
		}
		case '3':
		{
			_pauseSensor ^= true;
			break;
		}
	}


	keypressCtrl(key);

#ifdef _USE_SIMULATION_
	keypressSimulation(key);
#endif // _USE_SIMULATION_
}

//--------------------------------------------------------------
void ofApp::drawMessage()
{
	ofPushStyle();
	if (_motorCtrl)
	{
		ofSetColor(255, 0, 0);
		ofCircle(5, 5, 5);
	}

	if (_performanceMode)
	{
		ofSetColor(0, 255, 0);
		ofCircle(15, 5, 5);
	}

	if (_pauseSensor)
	{
		ofSetColor(0, 0, 255);
		ofCircle(25, 5, 5);
	}

	ofPopStyle();
}

#pragma region Control
//--------------------------------------------------------------
void ofApp::startCoin()
{
	if (_state != eCoinState::eWaitStart || _ctrlDevice.isOpen() || _pauseSensor)
	{
		return;
	}
	_videoMgr.stop();

	visionDisplay::GetInstance()->play();
	effectMusicMgr::GetInstance()->play();

	_counter.start(cCountTime);
	_idleVoice.stop();
	
	_state = eCoinState::eCoinPlay;

	_caller.addCMD(cCmd_ClearVideo, NAME_MGR::SCEvent_ClearVideo);
	_caller.signal();
}

//--------------------------------------------------------------
void ofApp::stopCoin()
{
	if (_state != eCoinState::eCoinPlay)
	{
		return;
	}
	effectMusicMgr::GetInstance()->stop();
	visionDisplay::GetInstance()->stop();

	_videoMgr.play(eVideoType::eVideoUploading);

	_openCountdown = cMotorCloseTime;
	_ctrlDevice.open();
	_coinSensorLeft.reset();
	_coinSensorRight.reset();
	_counter.stop();
	_state = eCoinState::eUploading;

}

//--------------------------------------------------------------
void ofApp::backtoTitle()
{
	if (_state != eCoinState::eUploading)
	{
		return;
	}
	_videoMgr.play(eVideoType::eVideoTutorial);
	_idleVoice.start();
	_state = eCoinState::eWaitStart;
}

//--------------------------------------------------------------
void ofApp::startPerformance()
{
	if (_state != eCoinState::eWaitStart || _ctrlDevice.isOpen() || _pauseSensor)
	{
		return;
	}
	cout << "startPerformance" << endl;
	_videoMgr.stop();

	visionDisplay::GetInstance()->play();
	effectMusicMgr::GetInstance()->play();

#ifdef _USE_SIMULATION_
	_diskL.startMusic();
	_diskR.startMusic();
#endif // _USE_SI_USE_SIMULATION_MULATION_
	_idleVoice.stop();
	
	_state = eCoinState::ePerformance;
}

//--------------------------------------------------------------
void ofApp::stopPerformance()
{
	if (_state != eCoinState::ePerformance)
	{
		return;
	}
	effectMusicMgr::GetInstance()->stop();
	visionDisplay::GetInstance()->stop();

	_videoMgr.play(eVideoType::eVideoTutorial);
	//_ctrlDevice.open();

#ifdef _USE_SIMULATION_
	_diskL.stopMusic();
	_diskR.stopMusic();
	_diskL.clearCoin();
	_diskR.clearCoin();
#endif // _USE_SIMULATION_



	_state = eCoinState::eWaitStart;
}
#pragma endregion

#pragma region Counter
//--------------------------------------------------------------
void ofApp::setupCounter()
{
	_counter.addCounterEvent(cShowHits, NAME_MGR::CEvent_ShowHint);
	_counter.addCounterEvent(cHideHits, NAME_MGR::CEvent_HideHint);

	_counter.addCounterEvent(cStartRecode, NAME_MGR::CEvent_StartRecode);
	_counter.addCounterEvent(cEndRecode, NAME_MGR::CEvent_EndRecode);
	_counter.addCounterEvent(cTriggerCoinFadeout, NAME_MGR::CEvent_StartFadeout);
	_counter.addCounterEvent(cEndingTime, NAME_MGR::CEvent_StartEnding);

	_counter.load("images/heart_icon.png", "font/ARIALNB.TTF");

	ofAddListener(_counter._countEvent, this, &ofApp::onCounterEvent);

	ofLog(OF_LOG_NOTICE, "[ofApp::setupCounter] Finish");
}

//--------------------------------------------------------------
void ofApp::updateCounter(float delta)
{
	_counter.update(delta);
}

//--------------------------------------------------------------
void ofApp::drawCounter()
{
	_counter.draw(cCounterRect);
}

//--------------------------------------------------------------
void ofApp::onCounterEvent(string& e)
{
	if (e == NAME_MGR::CEvent_ShowHint)
	{
		_videoMgr.play(eVideoType::eVideoHints);
	}
	else if (e == NAME_MGR::CEvent_HideHint)
	{
		_videoMgr.stop();
	}
	else if (e == NAME_MGR::CEvent_StartRecode)
	{
		simulatorKey::GetInstance()->sendKey(cStartRocodeKey);
	}
	else if (e == NAME_MGR::CEvent_EndRecode)
	{
		simulatorKey::GetInstance()->sendKey(cEndRocodeKey);
	}
	else if(e == NAME_MGR::CEvent_StartFadeout)
	{
		effectMusicMgr::GetInstance()->fadeout();
		visionDisplay::GetInstance()->fadeout();
	}
	else if (e == NAME_MGR::CEvent_StartEnding)
	{
		_videoMgr.play(eVideoType::eVideoEnding, false);
	}
	else
	{
		ofLog(OF_LOG_WARNING, "[ofApp::onCounterEvent]Unknow event type");
	}
}
#pragma endregion

#pragma region VideoMgr
//--------------------------------------------------------------
void ofApp::setupVideoMgr()
{
#ifdef _USE_SIMULATION_
	//_videoMgr.setDrawArea(ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.25), ofGetWidth() * 0.5, ofGetHeight() * 0.5);
#endif // _USE_SIMULATION_

	
	_videoMgr.addVideo(eVideoType::eVideoTutorial, "videos/tutorial.mov");
	_videoMgr.addVideo(eVideoType::eVideoHints, "videos/hits.mov");
	_videoMgr.addVideo(eVideoType::eVideoUploading, "videos/loading.mov");
	_videoMgr.addVideo(eVideoType::eVideoEnding, "videos/ending.mov");

	ofAddListener(_videoMgr._onVideoDone, this, &ofApp::onVideoDone);

	ofLog(OF_LOG_NOTICE, "[ofApp::setupVideoMgr] Finish");
}

//--------------------------------------------------------------
void ofApp::onVideoDone(eVideoType & eType)
{
	if (eType == eVideoType::eVideoEnding)
	{
		stopCoin();
		
		_userID = ofGetTimestampString("%m%d%H%M");
		_caller.addCMD(cCmd_MixVideo + _userID, NAME_MGR::SCEvent_MixVideo);
		_caller.signal();
	}
}
#pragma endregion

#pragma region Effect Music Manager
//--------------------------------------------------------------
void ofApp::loadAllMusic()
{
	string audioPath_ = config::GetInstance()->audioPath;

	effectMusicMgr::GetInstance()->loadBasic(audioPath_ + "Basic.wav");

	auto& channelData_ = config::GetInstance()->channelData;
	for (auto& Iter_ : channelData_)
	{
		effectMusicMgr::GetInstance()->addPlayer(Iter_._type
			,Iter_._audioGroup
			,Iter_._playerType
			,audioPath_ + Iter_._name + ".wav"
			,Iter_._extendTime);
	}

	effectMusicMgr::GetInstance()->setup();
	ofLog(OF_LOG_NOTICE, "[ofApp::loadAllMusic] Finish");
}

#pragma endregion

#pragma region Vision Display
//--------------------------------------------------------------
void ofApp::loadAllVideo()
{
	string videoPath_ = config::GetInstance()->videoPath;
	auto& channelData_ = config::GetInstance()->channelData;
	visionDisplay::GetInstance()->loadBasic(videoPath_ + "Basic.mov");

	for (auto& Iter_ : channelData_)
	{
		visionDisplay::GetInstance()->addPlayer(Iter_._type, Iter_._playerType, videoPath_ + Iter_ ._name + ".mov", Iter_._level, Iter_._extendTime);
	}

#ifdef _USE_SIMULATION_
	//visionDisplay::GetInstance()->setup(ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.25), ofGetWidth() * 0.5, ofGetHeight() * 0.5);
#endif // !_USE_SIMULATION_
	ofLog(OF_LOG_NOTICE, "[ofApp::loadAllVideo] Finish");
}

#pragma endregion

#pragma region Arduion
//--------------------------------------------------------------
void ofApp::setupArduino()
{
	_motorCtrl = false;
	_openCountdown = 0.0f;
	_ctrlDevice.setup(config::GetInstance()->devicePort, cSerialBaud);
	ofAddListener(_ctrlDevice._buttonEvent, this, &ofApp::onButtonEvent);
	ofAddListener(_ctrlDevice._sliderEvent, this, &ofApp::onSliderEvent);

	_coinSensorLeft.setup(config::GetInstance()->sensorLeftPort, cSerialBaud);
	_coinSensorRight.setup(config::GetInstance()->sensorRightPort, cSerialBaud);
	ofAddListener(_coinSensorLeft._sensorEvent, this, &ofApp::onSensorEvent);
	ofAddListener(_coinSensorRight._sensorEvent, this, &ofApp::onSensorEvent);

	ofLog(OF_LOG_NOTICE, "[ofApp::setupArduino] Finish");

}

//--------------------------------------------------------------
void ofApp::updateArduino(float delta)
{
	if (_ctrlDevice.isOpen())
	{
		_openCountdown -= delta;
		if (_openCountdown <= 0.0f)
		{
			_ctrlDevice.close();
		}
	}
}


//--------------------------------------------------------------
void ofApp::onButtonEvent(serialArgs<bool>& e)
{
	eButtonType buttonType_ = (eButtonType)(e._no);
	switch (buttonType_)
	{
		case eButton_Audio_1:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_1);
				visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_1);
			}			
			break;
		}
		case eButton_Audio_2:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_2);
				visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_2);
			}
			
			break;
		}
		case eButton_Audio_3:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_3);
				visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_3);
			}
			break;
		}
		case eButton_Audio_4:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_4);
				visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_4);
			}			
			break;
		}
		case eButton_Audio_5:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_5);
				visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_5);
			}
			break;
		}
		case eButton_Audio_6:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_6);
				visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_6);
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::onSliderEvent(serialArgs<float>& e)
{
	eSliderType eSliderType_ = (eSliderType)(e._no - '0');

	switch (eSliderType_)
	{
		case eSliderLeft:
		{
			float val_ = ofMap(e._param, 0.0f, 3.1f, 0.1f, 3.0f, true);
			effectMusicMgr::GetInstance()->setGroupVol(eAudioGroup::eAudioGroup_Left, val_);
			break;
		}
		case eSliderRight:
		{
			float val_ = ofMap(e._param, 0.0f, 3.1f, 0.1f, 3.0f, true);
			effectMusicMgr::GetInstance()->setGroupVol(eAudioGroup::eAudioGroup_Right, val_);
			break;
		}
		case eSliderCenter:
		{
			float val_ = ofMap(e._param, 0.0f, 3.0f, 0.0f, 1.0f, true);
			effectMusicMgr::GetInstance()->setGroupBalance(val_);
			break;
		}
		default:
		{
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::onSensorEvent(serialArgs<bool>& e)
{
	eAudioType audioType_ = (eAudioType)(e._no);
	if (audioType_ < eLeft_Channel_1 || audioType_ > eRight_Channel_5)
	{
		return;
	}

	if (e._param)
	{
		_performanceMode ? startPerformance() : startCoin();
		effectMusicMgr::GetInstance()->playerIn(audioType_);
		visionDisplay::GetInstance()->playerIn(audioType_);
	}
	else
	{
		effectMusicMgr::GetInstance()->playerOut(audioType_);
		visionDisplay::GetInstance()->playerOut(audioType_);
	}
	
}

//--------------------------------------------------------------
void ofApp::keypressCtrl(int key)
{
	if (!_motorCtrl)
	{
		return;
	}

	switch (key)
	{
		case 'o':
		case 'c':
		case 'q':
		case 'w':
		case 'e':
		case 'r':
		{
			_ctrlDevice.send(key);
		}
	}
}
#pragma endregion

#pragma region SystemCaller
//--------------------------------------------------------------
void ofApp::onSystemCallFinish(string& msg)
{
	if (msg == NAME_MGR::SCEvent_MixVideo)
	{
		ofLog(OF_LOG_NOTICE, "[ofApp::onSystemCallFinish]Start upload to server");
		upload(_userID);


	}
}
#pragma endregion

#pragma region Uploader

//--------------------------------------------------------------
void ofApp::upload(string name)
{
	string videoPath_ = ofFilePath::getCurrentExeDir() + "data/results/" + name + ".mp4";
	
	ofxHttpForm	httpForm_;
	httpForm_.action = cUPLOAD_URL;
	httpForm_.method = OFX_HTTP_POST;

	httpForm_.addFile("video", videoPath_);
	httpForm_.addFormField("id", name);
	httpForm_.addFormField("active", "uploadVideo");

	_httpConn.addForm(httpForm_);
}

//--------------------------------------------------------------
void ofApp::httpRespone(ofxHttpResponse & Response)
{
	if (Response.status == 200)
	{
		string strResult_ = Response.responseBody.getText();

		if (strResult_.find("true") != string::npos)
		{
			ofLog(OF_LOG_NOTICE, "[Uploader] uploader success : " + _userID);

			//Printer QR Code
			_qrPrinter.printQRCode(_userID);
		}
		else
		{
			ofLog(OF_LOG_ERROR, "[Uploader] uploader failed : " + _userID);
		}
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[Uploader] uploader internet error : " + Response.status);
	}

	backtoTitle();

}
#pragma endregion

#pragma region idle Voice
void ofApp::setupIdleVoice()
{
	_idleVoice.addVoice("voices/voice1.mp3");
	_idleVoice.addVoice("voices/voice2.mp3");
	_idleVoice.addVoice("voices/voice3.mp3");

	_idleVoice.setWaitTime(config::GetInstance()->idleVoiceTime);
	_idleVoice.setRange(config::GetInstance()->idleStartHour, config::GetInstance()->idleEndHour);
}
#pragma endregion

#ifdef _USE_SIMULATION_

#pragma region Simulation
//--------------------------------------------------------------
void ofApp::onCoinScanner(eAudioType & e)
{
	effectMusicMgr::GetInstance()->playerToggle(e);
	visionDisplay::GetInstance()->playerToggle(e);
}
//--------------------------------------------------------------
void ofApp::setupSimulation()
{
	ofSetCircleResolution(50);

	//Disk
	_diskL.setup(400);
	_diskL.addChannel(eLeft_Channel_1, 200);
	_diskL.addChannel(eLeft_Channel_2, 160);
	_diskL.addChannel(eLeft_Channel_3, 120);
	_diskL.addChannel(eLeft_Channel_4, 80);
	_diskL.addChannel(eLeft_Channel_5, 40);

	_diskR.setup(400);
	_diskR.addChannel(eRight_Channel_1, 200);
	_diskR.addChannel(eRight_Channel_2, 160);
	_diskR.addChannel(eRight_Channel_3, 120);
	_diskR.addChannel(eRight_Channel_4, 80);
	_diskR.addChannel(eRight_Channel_5, 40);

	//Button
	_buttons.setup(400, 200, 2, 3);
	_buttons.addButton(eButton_Audio_1);
	_buttons.addButton(eButton_Audio_2);
	_buttons.addButton(eButton_Audio_3);
	_buttons.addButton(eButton_Audio_4);
	_buttons.addButton(eButton_Audio_5);
	_buttons.addButton(eButton_Audio_6);

	ofAddListener(channel::_channelToggle, this, &ofApp::onCoinScanner);
}

//--------------------------------------------------------------
void ofApp::updateSimulation(float delta)
{
	_diskL.update(delta);
	_diskR.update(delta);
}

//--------------------------------------------------------------
void ofApp::drawSimulation()
{
	_diskL.draw(ofVec2f(ofGetWidth() / 4, ofGetHeight() / 4 * 3));
	_diskR.draw(ofVec2f(ofGetWidth() / 4 * 3, ofGetHeight() / 4 * 3));

	_buttons.draw(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 4 * 3));
}

//--------------------------------------------------------------
void ofApp::keypressSimulation(int key)
{
	switch (key)
	{
	case 'a':
	{
		_diskL.startMusic();
		_diskR.startMusic();
		break;
	}
	case 's':
	{
		_diskL.stopMusic();
		_diskR.stopMusic();
		break;
	}
	case '4':
	{
		_diskL.clearCoin();
		_diskR.clearCoin();
		break;
	}
	//effect 
	case 'q':
	{
		_diskL.coin(eAudioType::eLeft_Channel_1);
		break;
	}
	case 'w':
	{
		_diskL.coin(eAudioType::eLeft_Channel_2);
		break;
	}
	case 'e':
	{
		_diskL.coin(eAudioType::eLeft_Channel_3);

		break;
	}
	case 'r':
	{
		_diskL.coin(eAudioType::eLeft_Channel_4);
		break;
	}
	case 't':
	{
		_diskL.coin(eAudioType::eLeft_Channel_5);
		break;
	}
	case 'y':
	{
		_diskR.coin(eAudioType::eRight_Channel_1);
		break;
	}
	case 'u':
	{
		_diskR.coin(eAudioType::eRight_Channel_2);
		break;
	}
	case 'i':
	{
		_diskR.coin(eAudioType::eRight_Channel_3);
		break;
	}
	case 'o':
	{
		_diskR.coin(eAudioType::eRight_Channel_4);
		break;
	}
	case 'p':
	{
		_diskR.coin(eAudioType::eRight_Channel_5);
		break;
	}

	//Button Audio
	case 'h':
	{
		effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_1);
		visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_1);
		_buttons.troggleButton(eButton_Audio_1);
		break;
	}
	case 'j':
	{
		effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_2);
		visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_2);
		_buttons.troggleButton(eButton_Audio_2);
		break;
	}
	case 'k':
	{
		effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_3);
		visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_3);
		_buttons.troggleButton(eButton_Audio_3);
		break;
	}
	case 'b':
	{
		effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_4);
		visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_4);
		_buttons.troggleButton(eButton_Audio_4);
		break;
	}
	case 'n':
	{
		effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_5);
		visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_5);
		_buttons.troggleButton(eButton_Audio_5);
		break;
	}
	case 'm':
	{
		effectMusicMgr::GetInstance()->playerIn(eAudioType::eCenter_Button_6);
		visionDisplay::GetInstance()->playerIn(eAudioType::eCenter_Button_6);
		_buttons.troggleButton(eButton_Audio_6);
		break;
	}
	}
}
#pragma endregion

#endif // _USE_SIMULATION_
