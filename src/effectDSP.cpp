#include "effectDSP.h"

#pragma region effectDSP Basic
//--------------------------------------------------------------
void effectDSP::setup(FMOD_DSP_TYPE type)
{

	FMOD_RESULT result_ = ofFmodGetSystem()->createDSPByType(type, &_dspEffect);
	result_ = ofFmodGetChannelGroup()->addDSP(0, _dspEffect);
	
	if (result_ == FMOD_OK)
	{
		initialDSP();
		_isSetup = true;
		_dspEffect->setBypass(true);
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[effectDSP - setup]Setup Failed");
	}
}

//--------------------------------------------------------------
void effectDSP::setActive(bool isActive)
{
	if (_isSetup)
	{
		if (isActive)
		{
			startDSP();
		}

		_dspEffect->setBypass(!isActive);
		_isActive = isActive;
	}
}

//--------------------------------------------------------------
void effectDSP::troggle()
{
	if (_isSetup)
	{
		_isActive ^= true;
		setActive(_isActive);
	}
}

//--------------------------------------------------------------
void effectDSP::initialDSP()
{}

//--------------------------------------------------------------
void effectDSP::startDSP()
{
}

//--------------------------------------------------------------
void effectDSP::update(float delta)
{}
#pragma endregion

#pragma region echoDSP
void echoDSP::initialDSP()
{
	_dspEffect->setParameterFloat(FMOD_DSP_ECHO_DELAY, 630);
	_dspEffect->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20.0f);
	_dspEffect->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, -5.0f);
	_dspEffect->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL, 0.0f);
}
#pragma endregion

#pragma region distortionDSP
void distortionDSP::initialDSP()
{
	_dspEffect->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, 0.7);
}
#pragma endregion

#pragma region lowpassDSP
//--------------------------------------------------------------
void lowpassDSP::initialDSP()
{
	_animCutHz.setRepeatType(AnimRepeat::LOOP_BACK_AND_FORTH);
	_animCutHz.setDuration(2.0);

	_dspEffect->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 500);
	_dspEffect->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 1.5);
}

//--------------------------------------------------------------
void lowpassDSP::startDSP()
{
	_animCutHz.animateFromTo(500, 3000.0);
}

//--------------------------------------------------------------
void lowpassDSP::update(float delta)
{
	if (_isActive)
	{
		_animCutHz.update(delta);
		_dspEffect->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, _animCutHz.getCurrentValue());
	}
}
#pragma endregion
