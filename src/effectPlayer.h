#pragma once

#include "constParameter.h"
#include "fmod.hpp"

class basicPlayer
{
public:
	basicPlayer()
	{}

	bool load(string path, bool loop = true);
	virtual void update(float delta) {};
	virtual void play() {};
	virtual void stop() {};

	virtual void in() {};
	virtual void out() {};

	virtual void setMaxVol(float vol) {};
	virtual float getMaxVol() { return 1.0f; };
	virtual bool isPlaying() { return false; };
protected:
	
	ofSoundPlayer _player;
};

class loopingPlayer : public basicPlayer
{
public:
	loopingPlayer(float extendLength);
	~loopingPlayer();
	
	void update(float delta) override;
	void play() override;
	void stop() override;
	
	void in() override;
	void out() override;
	bool isPlaying() override;

	void setMaxVol(float vol) override;
	float getMaxVol() override;
	eFadeState getState() const;

private:
	float _timer, _extendLength;
	float _vol, _maxVol;
	eFadeState _eState;

};

class triggerPlayer : public basicPlayer
{
public:
	triggerPlayer();
	~triggerPlayer();

	void play() override;
	void stop() override;

	void in() override;
	void out() override;
	bool isPlaying() override;

private:
	bool _isStart;
};