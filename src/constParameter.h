#pragma once


//#define _USE_SIMULATION_
#include "ofMain.h"

//------------------------------------
//ENUM
//------------------------------------
enum ePlayerType
{
	eLoopingPlayer = 0
	,eTriggerPlayer
};

enum eAudioType
{
	eLeft_Channel_1 = 0
	, eLeft_Channel_2
	, eLeft_Channel_3
	, eLeft_Channel_4
	, eLeft_Channel_5
	, eRight_Channel_1
	, eRight_Channel_2
	, eRight_Channel_3
	, eRight_Channel_4
	, eRight_Channel_5

	, eCenter_Button_1 = 20
	, eCenter_Button_2
	, eCenter_Button_3
};

enum eAudioGroup
{
	eAudioGroup_Left = 0
	,eAudioGroup_Right
	,eAudioGroup_Button

};

enum eFadeState
{
	eStateWait = 0
	, eStateEnter
	, eStatePlay
	, eStateExit
	, eStateUnknow
};

enum eButtonType
{
	eButton_Audio_1 = 0
	, eButton_Audio_2
	, eButton_Audio_3
	, eButton_Effect_Echo
	, eButton_Effect_Dist
	, eButton_Effect_LowPass
};

enum eSliderType {
	eSliderLeft = 0
	,eSliderRight
	,eSliderCenter
};

const static string cConfig_File_Path = "";

//------------------------------------
//Serial
//------------------------------------
const static int cSerialBaud = 9600;
const static int cSerialReadNum = 25;
const static int cSerialBufferNum = 25;
const static string cSerialMsgSTX = "\x01";
const static string cSerialMsgEND = "\x02";

const static int cBtnNum = 6;

const static int cCoinSetNum = 2;
const static int cCoinNumEachSet = 5;//Each set
const static int cCoinNum = cCoinNumEachSet * cCoinSetNum;

const static int cMotorCloseTime = 6.0f;

//------------------------------------
//Disk
//------------------------------------
const static float cCoinRadius = 15.0f;
const static float cScannerAndCoinDegree = 180.0f;
