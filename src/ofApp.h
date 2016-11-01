#pragma once


#include "configData.h"
#include "effectMusicMgr.h"
#include "visionDisplay.h"

#include "disk.h"
#include "buttonArray.h"
#include "midiParameter.h"
#include "serialCtrl.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

private:
	float _mainTimer;

#pragma region Effect Music Manager
private:
	void loadAllMusic();

#pragma endregion

#pragma region Vision Display
private:
	void loadAllVideo();
#pragma endregion

#pragma region Arduino
public:
	void setupArduino();
	void onButtonEvent(serialArgs<bool>& e);
	void onSliderEvent(serialArgs<float>& e);
	void onSensorEvent(serialArgs<bool>& e);
private:
	deviceCtrl	_ctrlDevice;
	sensorCtrl	_coinSensorLeft, _coinSensorRight;
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
