#pragma once

#include "ofxHapPlayer.h"
#include "ofxAnimatableFloat.h"
#include "constParameter.h"

class videoMgr
{
public:
	videoMgr();
	~videoMgr();
	void update(float delta);
	void draw();

	void play(eVideoType type, bool isLoop = true);
	void stop();

	void addVideo(eVideoType type, string path);
	void setDrawArea(ofVec2f center, int width, int height);

private:
	bool _isPlay, _checkFinish;
	ofRectangle	_drawArea;
	eVideoType _ePlayType;
	ofPtr<ofVideoPlayer> _videoPlay;
	map<eVideoType, ofPtr<ofVideoPlayer>> _videoSet;

//----------------------------------
//EVENT
public:
	ofEvent<eVideoType> _onVideoDone;
};