#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

#include "fmod.hpp"


enum eEffectType
{
	eEffect_Echo = 0
	, eEffect_Distortion
	, eEffect_PitchShift
	, eEffect_LowPass
	, eEffect_HighPass
};

class effectDSP
{
public:
	effectDSP()
		:_dspEffect(nullptr)
		,_isSetup(false)
		,_isActive(false)
	{}
	~effectDSP()
	{
		_dspEffect->release();
	}

	void setup(FMOD_DSP_TYPE type);
	void setActive(bool isActive);
	void troggle();
	virtual void update(float delta);

protected:
	virtual void initialDSP();
	virtual void startDSP();

protected:
	bool	_isSetup;
	bool	_isActive;
	FMOD::DSP *_dspEffect;
};

//---------------------------------------------
class echoDSP : public effectDSP
{
public:
	void initialDSP() override;
};

//---------------------------------------------
class distortionDSP : public effectDSP
{
public:
	void initialDSP() override;
};

//---------------------------------------------
class lowpassDSP : public effectDSP
{
public:
	void initialDSP() override;
	void startDSP() override;
	void update(float delta) override;

private:
	ofxAnimatableFloat _animCutHz;
};

//---------------------------------------------
class pitchShifterDSP : public effectDSP
{
public:
	void initialDSP() override;

};

