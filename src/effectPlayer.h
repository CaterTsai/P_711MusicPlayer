#pragma once

#include "constParameter.h"
#include "fmod.hpp"

class basicPlayer
{
public:
	basicPlayer()
		:_maxVol(1.0f)
	{}

	bool load(string path, bool loop = true);
	virtual void update(float delta) {};
	virtual void play() {};
	virtual void stop() {};

	virtual void in() {};
	virtual void out() {};

	virtual void setMaxVol(float vol) {
		_maxVol = vol;
		if (isPlaying())
		{
			_player.setVolume(_maxVol);
		}
	};
	virtual float getMaxVol() { return 1.0f; };
	virtual bool isPlaying() { return false; };
protected:
	float _maxVol;
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

	float getMaxVol() override;
	eFadeState getState() const;

private:
	float _timer, _extendLength;
	float _vol;
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