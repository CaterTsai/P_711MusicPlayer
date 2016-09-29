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
	_dspEffect->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300.0f);
	_dspEffect->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20.0f);
	_dspEffect->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, -20.0f);
}
#pragma endregion

#pragma region distortionDSP
void distortionDSP::initialDSP()
{
	_dspEffect->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, 0.7);
}
#pragma endregion

#pragma region pitchShiftDSP
//--------------------------------------------------------------
void pitchShiftDSP::initialDSP()
{
	//_rotateDegree = 0.0;
	//_rotateDegreeV = 360.0 / 5.0;
	_dspEffect->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7);
}

////--------------------------------------------------------------
//void pitchShiftDSP::startDSP()
//{
//	_rotateDegree = 0.0;
//	_rotateDegreeV = 360.0 / 30.0;
//}

////--------------------------------------------------------------
//void pitchShiftDSP::update(float delta)
//{
//	if (_isActive)
//	{
//		_rotateDegree += _rotateDegreeV * delta;
//		if (_rotateDegree >= 360.0)
//		{
//			_rotateDegree -= 360.0;
//		}
//
//		float val_ = ofMap(sin( DEG_TO_RAD * _rotateDegree ), 0.0f, 1.0f, 1.0f, 2.0f);
//		_dspEffect->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, val_);
//	}
//}
#pragma endregion


