#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	config::GetInstance()->load("_config.xml");
	_isStartCoin = false;
	loadAllMusic();
	loadAllVideo();

	//Arduino
	setupArduino();

#ifdef _USE_SIMULATION_
	setupSimulation();
#endif // _USE_SIMULATION_

	ofBackground(0);
	ofSetVerticalSync(true);
	ofToggleFullscreen();

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

	updateArduino(delta_);

#ifdef _USE_SIMULATION_
	updateSimulation(delta_);
#endif // _USE_SIMULATION_


	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){

	visionDisplay::GetInstance()->draw();

	drawArduino();
#ifdef _USE_SIMULATION_
	drawSimulation();
#endif // _USE_SIMULATION_
}

//--------------------------------------------------------------
void ofApp::exit()
{

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
		
		//Button Audio
		case 'h':
		{
			effectMusicMgr::GetInstance()->playerToggle(eAudioType::eCenter_Button_1);
			visionDisplay::GetInstance()->playerToggle(eAudioType::eCenter_Button_1);
			break;
		}
		case 'j':
		{
			effectMusicMgr::GetInstance()->playerToggle(eAudioType::eCenter_Button_2);
			visionDisplay::GetInstance()->playerToggle(eAudioType::eCenter_Button_2);
			break;
		}
		case 'k':
		{
			effectMusicMgr::GetInstance()->playerToggle(eAudioType::eCenter_Button_3);
			visionDisplay::GetInstance()->playerToggle(eAudioType::eCenter_Button_3);
			break;
		}

		//Button Effect
		case 'b':
		{
			effectMusicMgr::GetInstance()->troggleEffect(eEffect_Echo);
			break;
		}
		case 'n':
		{
			effectMusicMgr::GetInstance()->troggleEffect(eEffect_Distortion);
			break;
		}
		case 'm':
		{
			effectMusicMgr::GetInstance()->troggleEffect(eEffect_LowPass);
			break;
		}

		//Draw Area Chage
		case 'z':
		{
			visionDisplay::GetInstance()->setDrawArea(ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.25), ofGetWidth() * 0.5, ofGetHeight() * 0.5);
			break;
		}
		case 'x':
		{
			visionDisplay::GetInstance()->setDrawArea(ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.5), ofGetWidth(), ofGetHeight());
			break;
		}

		//Fullscreen
		case 'f':
		{
			ofToggleFullscreen();
			break;
		}

		//Ctrl
		case '1':
		{
			_coinSensorLeft.reset();
			_coinSensorRight.reset();
			ofLog(OF_LOG_NOTICE, "[Keypress]Reset coin sensor");
			break;
		}
		case '2':
		{	
			_motorCtrl ^= true;
			break;
		}
	}


	keypressCtrl(key);

#ifdef _USE_SIMULATION_
	keypressSimulation(key);
#endif // _USE_SIMULATION_


}

#pragma region Control
//--------------------------------------------------------------
void ofApp::startCoin()
{
	if (_isStartCoin && !_ctrlDevice.isOpen())
	{
		return;
	}
	visionDisplay::GetInstance()->stopTutorial();

	effectMusicMgr::GetInstance()->playBasic();
	effectMusicMgr::GetInstance()->play();
	visionDisplay::GetInstance()->playBasic();
	visionDisplay::GetInstance()->play();

	_isStartCoin = true;
}

//--------------------------------------------------------------
void ofApp::stopCoin()
{
	if (!_isStartCoin)
	{
		return;
	}
	effectMusicMgr::GetInstance()->stopBasic();
	effectMusicMgr::GetInstance()->stop();
	visionDisplay::GetInstance()->stopBasic();
	visionDisplay::GetInstance()->stop();

	visionDisplay::GetInstance()->playTutorial();

	_openCountdown = cMotorCloseTime;
	_ctrlDevice.open();

	_isStartCoin = false;
}
#pragma endregion

#pragma region Effect Music Manager
//--------------------------------------------------------------
void ofApp::loadAllMusic()
{
	string audioPath_ = config::GetInstance()->audioPath;

	effectMusicMgr::GetInstance()->loadBasic(audioPath_ + "Basic.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Channel_1, eAudioGroup::eAudioGroup_Left, ePlayerType::eLoopingPlayer, audioPath_ + "L_WaterFall.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Channel_2, eAudioGroup::eAudioGroup_Left, ePlayerType::eLoopingPlayer, audioPath_ + "L_Forest.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Channel_3, eAudioGroup::eAudioGroup_Left, ePlayerType::eLoopingPlayer, audioPath_ + "L_MRT.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Channel_4, eAudioGroup::eAudioGroup_Left, ePlayerType::eLoopingPlayer, audioPath_ + "L_Space.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Channel_5, eAudioGroup::eAudioGroup_Left, ePlayerType::eLoopingPlayer, audioPath_ + "L_WindBell.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Channel_1, eAudioGroup::eAudioGroup_Right, ePlayerType::eLoopingPlayer, audioPath_ + "R_BASS.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Channel_2, eAudioGroup::eAudioGroup_Right, ePlayerType::eLoopingPlayer, audioPath_ + "R_DrumSet.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Channel_3, eAudioGroup::eAudioGroup_Right, ePlayerType::eLoopingPlayer, audioPath_ + "R_GT.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Channel_4, eAudioGroup::eAudioGroup_Right, ePlayerType::eLoopingPlayer, audioPath_ + "R_Synth.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Channel_5, eAudioGroup::eAudioGroup_Right, ePlayerType::eLoopingPlayer, audioPath_ + "R_Crash.wav");

	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eCenter_Button_1, eAudioGroup::eAudioGroup_Button, ePlayerType::eTriggerPlayer, audioPath_ + "B_TomRoll.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eCenter_Button_2, eAudioGroup::eAudioGroup_Button, ePlayerType::eTriggerPlayer, audioPath_ + "B_JINGO.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eCenter_Button_3, eAudioGroup::eAudioGroup_Button, ePlayerType::eTriggerPlayer, audioPath_ + "B_Scartch.wav");

	effectMusicMgr::GetInstance()->setup();
}

#pragma endregion

#pragma region Vision Display
//--------------------------------------------------------------
void ofApp::loadAllVideo()
{
	string videoPath_ = config::GetInstance()->videoPath;

	visionDisplay::GetInstance()->loadBasic(videoPath_ + "Basic.mov");
	visionDisplay::GetInstance()->loadTutorial(videoPath_ + "tutorial.mov");

	visionDisplay::GetInstance()->addPlayer(eAudioType::eLeft_Channel_1, ePlayerType::eLoopingPlayer, videoPath_ + "L_WaterFall.mov", 9);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eLeft_Channel_2, ePlayerType::eLoopingPlayer, videoPath_ + "L_Forest.mov", 2);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eLeft_Channel_3, ePlayerType::eLoopingPlayer, videoPath_ + "L_MRT.mov", 0);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eLeft_Channel_4, ePlayerType::eLoopingPlayer, videoPath_ + "L_Space.mov", 1);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eLeft_Channel_5, ePlayerType::eLoopingPlayer, videoPath_ + "L_WindBell.mov", 7);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eRight_Channel_1, ePlayerType::eLoopingPlayer, videoPath_ + "R_BASS.mov", 5);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eRight_Channel_2, ePlayerType::eLoopingPlayer, videoPath_ + "R_DrumSet.mov", 3);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eRight_Channel_3, ePlayerType::eLoopingPlayer, videoPath_ + "R_GT.mov", 4);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eRight_Channel_4, ePlayerType::eLoopingPlayer, videoPath_ + "R_Synth.mov", 8);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eRight_Channel_5, ePlayerType::eLoopingPlayer, videoPath_ + "R_Crash.mov", 6);
	
	visionDisplay::GetInstance()->addPlayer(eAudioType::eCenter_Button_1, ePlayerType::eTriggerPlayer, videoPath_ + "B_TomRoll.mov", 11);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eCenter_Button_2, ePlayerType::eTriggerPlayer, videoPath_ + "B_JINGO.mov", 10);
	visionDisplay::GetInstance()->addPlayer(eAudioType::eCenter_Button_3, ePlayerType::eTriggerPlayer, videoPath_ + "B_Scartch.mov", 12);

#ifndef _USE_SIMULATION_
	visionDisplay::GetInstance()->setup(ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.5), ofGetWidth(), ofGetHeight());
#else //_USE_SIMULATION_
	visionDisplay::GetInstance()->setup(ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.25), ofGetWidth() * 0.5, ofGetHeight() * 0.5);
#endif // !_USE_SIMULATION_

	visionDisplay::GetInstance()->playTutorial();
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
void ofApp::drawArduino()
{
	if (_motorCtrl)
	{
		ofPushStyle();
		ofSetColor(255, 0, 0);
		ofCircle(10, 10, 5);
		ofPopStyle();
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
			else
			{
				effectMusicMgr::GetInstance()->playerOut(eAudioType::eCenter_Button_1);
				visionDisplay::GetInstance()->playerOut(eAudioType::eCenter_Button_1);
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
			else
			{
				effectMusicMgr::GetInstance()->playerOut(eAudioType::eCenter_Button_2);
				visionDisplay::GetInstance()->playerOut(eAudioType::eCenter_Button_2);
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
			else
			{
				effectMusicMgr::GetInstance()->playerOut(eAudioType::eCenter_Button_3);
				visionDisplay::GetInstance()->playerOut(eAudioType::eCenter_Button_3);
			}

			break;
		}
		case eButton_Effect_Echo:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->activeEffect(eEffect_Echo);
			}
			else
			{
				effectMusicMgr::GetInstance()->unactiveEffect(eEffect_Echo);
			}
			
			break;
		}
		case eButton_Effect_Dist:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->activeEffect(eEffect_Distortion);
			}
			else
			{
				effectMusicMgr::GetInstance()->unactiveEffect(eEffect_Distortion);
			}
			
			break;
		}
		case eButton_Effect_LowPass:
		{
			if (e._param)
			{
				effectMusicMgr::GetInstance()->activeEffect(eEffectType::eEffect_LowPass);
			}
			else
			{
				effectMusicMgr::GetInstance()->unactiveEffect(eEffectType::eEffect_LowPass);
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
		if (!_isStartCoin)
		{
			//Start Coin Check
			startCoin();
		}

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
	_buttons.addButton(eButton_Effect_Echo);
	_buttons.addButton(eButton_Effect_Dist);
	_buttons.addButton(eButton_Effect_LowPass);

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
	case 'c':
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
		_buttons.troggleButton(eButton_Audio_1);
		break;
	}
	case 'j':
	{
		_buttons.troggleButton(eButton_Audio_2);
		break;
	}
	case 'k':
	{
		_buttons.troggleButton(eButton_Audio_3);
		break;
	}

	//Button Effect
	case 'b':
	{
		_buttons.troggleButton(eButton_Effect_Echo);
		break;
	}
	case 'n':
	{
		_buttons.troggleButton(eButton_Effect_Dist);
		break;
	}
	case 'm':
	{
		_buttons.troggleButton(eButton_Effect_LowPass);
		break;
	}
	}
}
#pragma endregion

#endif // _USE_SIMULATION_
