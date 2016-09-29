#pragma once

#include "ofMain.h"
#include "fmod.hpp"

enum eEffectFadeState
{
	eStateMute	=	0
	,eStateEnter
	,eStatePlay
	,eStateExit
	,eStateUnknow
};

class effectPlayer
{
public:
	effectPlayer();
	~effectPlayer();
	
	bool load(string path, bool loop = true);
	void play();
	void stop();
	
	void in();
	void out();

	eEffectFadeState getState() const;

private:
	eEffectFadeState _eState;
	float _vol;
	ofSoundPlayer _player;	
};