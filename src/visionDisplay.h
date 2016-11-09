#pragma once

#include "constParameter.h"
#include "visionPlayer.h"
#include "ofxAnimatableFloat.h"

class visionDisplay
{
public:
	void setup(ofVec2f center, int width, int height);
	void update(float delta);
	void draw();
	bool isPlaying();

	void play();
	void stop();

	void setDrawArea(ofVec2f center, int width, int height);

private:
	bool _isVisionStart;
	ofRectangle	_drawArea;

#pragma region Fader
public:
	void fadeout();
	void fadein();
private:
	void updateFader(float delta);
	void drawFader();
private:
	ofxAnimatableFloat	_animFadeAlpha;
#pragma endregion

#pragma region Basic
public:
	void loadBasic(string path);

private:
	void updateBasic();
	void drawBasic();
private:
	ofVideoPlayer	_basicPlayer;
#pragma endregion
	
#pragma region vision Player
public:
	void addPlayer(eAudioType eType, ePlayerType ePlayerType, string path, int level, float extendTime = 2.0f);

	bool getIsPlaying(eAudioType eType);
	void playerToggle(eAudioType eType);
	void playerIn(eAudioType eType);
	void playerOut(eAudioType eType);

private:
	void updateAllPlayer(float delta);
	void drawAllPlayer(int x, int y, int width, int height);


private:
	map<eAudioType, ofPtr<basicVisionPlayer>>	_playerMgr;
	vector<eAudioType> _drawLevel;
#pragma endregion

#pragma region Singleton
	//-------------------
	//Singleton
	//-------------------
private:
	visionDisplay()
		:_isVisionStart(false)
	{
		_drawArea.set(ofVec2f(0, 0), ofGetWidth(), ofGetHeight());
		_drawLevel.resize(eAudioNum);
	};
	~visionDisplay()
	{
		visionDisplay::Destroy();
	}
	visionDisplay(visionDisplay const&) {};
	void operator=(visionDisplay const&) {};

public:
	static visionDisplay* GetInstance();
	static void Destroy();

private:
	static visionDisplay *pInstance;
#pragma endregion

};
