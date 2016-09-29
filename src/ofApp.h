#pragma once

#include "ofxMidi.h"

#include "configData.h"
#include "effectMusicMgr.h"

#include "disk.h"
#include "buttonArray.h"
#include "midiParameter.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

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


#pragma region Simulation
//Disk
private:
	void setupSimulation();
	void updateSimulation(float delta);
	void drawSimulation();

private:
	disk _diskL, _diskR;
	buttonArray	_buttons;
#pragma endregion

#pragma region Midi KB
private:
	void setupMidi();
	void exitMidi();
	void newMidiMessage(ofxMidiMessage& msg) override;

private:
	map<int, eAudioType> _midiMapper;
	ofxMidiIn	_midiIn;
#pragma endregion




};
