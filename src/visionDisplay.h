#pragma once

#include "constParameter.h"
#include "visionPlayer.h"


class visionDisplay
{
public:
	void setup(ofVec2f center, int width, int height);
	void update(float delta);
	void draw();
	bool isPlaying();

	void setDrawArea(ofVec2f center, int width, int height);

private:
	bool _isVisionStart;
	ofRectangle	_drawArea;

#pragma region Basic
public:
	void loadBasic(string path);
	void playBasic();
	void stopBasic();

private:
	void updateBasic();
	void drawBasic();
private:
	ofVideoPlayer	_basicPlayer;
#pragma endregion
	
#pragma region vision Player
public:
	void addPlayer(eAudioType eType, ePlayerType ePlayerType, string path, int level);
	void play();
	void stop();

	bool getIsPlaying(eAudioType eType);
	void playerToggle(eAudioType eType);
	void playerIn(eAudioType eType);
	void playerOut(eAudioType eType);

private:
	void updateAllPlayer(float delta);
	void drawAllPlayer(int x, int y, int width, int height);

private:
	struct playerCompare
	{
		bool operator()(const loopingVisionPlayer& a, const loopingVisionPlayer& b)
		{
			return a.getDrawLevel() < b.getDrawLevel();
		}
	};

private:
	map<eAudioType, ofPtr<basicVisionPlayer>, playerCompare>	_playerMgr;
#pragma endregion

#pragma region Tutorial
public:
	void loadTutorial(string path);
	void playTutorial();
	void stopTutorial();

private:
	
	void updateTutorial();
	void drawTutorial();
	
private:
	ofVideoPlayer	_tutorial;


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
