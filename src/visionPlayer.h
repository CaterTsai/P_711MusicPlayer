#pragma once

#include "constParameter.h"
#include "ofxHapPlayer.h"

class basicVisionPlayer
{
public:
	basicVisionPlayer(int drawLevel = 0);
	~basicVisionPlayer();

	bool load(string path);
	void update(float delta);
	void draw(int x, int y, int width, int height);
	int getDrawLevel() const;
	
	virtual void play() {};
	virtual void stop() {};

	virtual void in() {};
	virtual void out() {};

	virtual bool isPlaying() { return false; };
protected:
	ofVideoPlayer	_player;
	int				_drawLevel;
	float			_alpha;
};

//-------------------------------
class loopingVisionPlayer : public basicVisionPlayer
{
public:
	loopingVisionPlayer(int drawLevel = 0);
	~loopingVisionPlayer();

	void play() override;
	void stop() override;

	void in() override;
	void out() override;

	bool isPlaying() override;
	eFadeState getState() const;
	

private:
	eFadeState _eState;
};

//-------------------------------
class triggerVisionPlayer : public basicVisionPlayer
{
public:
	triggerVisionPlayer(int drawLevel = 0);
	void play() override;
	void stop() override;

	void in() override;
	void out() override;

	bool isPlaying() override;

private:
	bool _isStart;
};