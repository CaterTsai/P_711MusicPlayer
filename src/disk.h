#pragma once

#include "constParameter.h"
#include "effectMusicMgr.h"

class disk
{
private:

public:
	disk()
		:_isStart(false)
		,_maxSize(0)
		, _channalSize(0)
		, _rotateV(36)
		, _rotateDegree(0.0)
	{}
	void setup(int size);
	void update(float delta);
	void draw(ofVec2f center);

	void addChannal(eAudioType type);

	void startMusic();
	void stopMusic();

	void channalOn(eAudioType type);
	void channalOff(eAudioType type);

private:
	void updateChannalSize();

private:
	bool _isStart;
	int _maxSize, _channalSize;
	float _rotateV, _rotateDegree;
	map<eAudioType, bool> _diskMgr;
};