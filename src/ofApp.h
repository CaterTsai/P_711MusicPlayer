#pragma once

#include "ofxHttpUtils.h"

//Singleton
#include "configData.h"
#include "effectMusicMgr.h"
#include "visionDisplay.h"
#include "simulatorKey.h"

//---------------------
#include "QRPrinter.h"
#include "serialCtrl.h"
#include "counter.h"
#include "ofxCTSystemCaller.h"


#ifdef _USE_SIMULATION_
#include "disk.h"
#include "buttonArray.h"
#endif // _USE_SIMULATION_


class ofApp : public ofBaseApp {

public:
	ofApp()
		:_state(eWaitStart)
		, _muteBasic(false)
	{}
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

private:
	float _mainTimer;
	string _userID;

#pragma region Control
public:
	void startCoin();
	void stopCoin();
	void backtoTitle();
private:
	enum eCoinState
	{
		eWaitStart = 0
		,eCoinPlay
		,eUploading
	}_state;

#pragma endregion

#pragma region Counter
public:
	void setupCounter();
	void updateCounter(float delta);
	void drawCounter();

	void onCounterEvent(string& e);
private:
	counter	_counter;
#pragma endregion

#pragma region Effect Music Manager
private:
	void loadAllMusic();

private:
	bool _muteBasic;
#pragma endregion

#pragma region Vision Display
private:
	void loadAllVideo();
#pragma endregion

#pragma region Arduino
public:
	void setupArduino();
	void updateArduino(float delta);
	void drawArduino();
	void onButtonEvent(serialArgs<bool>& e);
	void onSliderEvent(serialArgs<float>& e);
	void onSensorEvent(serialArgs<bool>& e);

private:
	void keypressCtrl(int key);

private:
	bool _motorCtrl;
	float _openCountdown;
	deviceCtrl	_ctrlDevice;
	sensorCtrl	_coinSensorLeft, _coinSensorRight;
#pragma endregion

#pragma region SystemCaller
public:
	void onSystemCallFinish(string& msg);
private:
	ofxCTSystemCaller	_caller;
#pragma endregion

#pragma region Uploader
public:
	void upload(string name);
	void httpRespone(ofxHttpResponse& Response);
private:
	ofxHttpUtils		_httpConn;
#pragma endregion

#pragma region QR Printer
private:
	qrPrinter	_qrPrinter;
#pragma endregion


#pragma region Simulation
#ifdef _USE_SIMULATION_
//Disk
public:
	void onCoinScanner(eAudioType& e);

private:
	void setupSimulation();
	void updateSimulation(float delta);
	void drawSimulation();
	void keypressSimulation(int key);

private:
	disk _diskL, _diskR;
	buttonArray	_buttons;
#endif // _USE_SIMULATION_
#pragma endregion

};
