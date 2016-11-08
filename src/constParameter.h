#pragma once


#define _USE_SIMULATION_
#include "ofMain.h"
#include "NameManager.h"
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

	, eCenter_Button_1
	, eCenter_Button_2
	, eCenter_Button_3

	, eAudioNum
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
	, eStateExtend
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

const static float cTriggerCoinFadeout = 3.0;
const static float cCoinFadeinTime = 1.0;
const static float cCoinFadeoutTime = 3.0;

//Video
const static string cVideoTempPath = "data/recodeTemp/";
const static float cRecodeLength = 35.0f;
const static float cStartRecode = 50.0f;
const static float cEndRecode = cStartRecode - cRecodeLength;


//------------------------------------
//Countdown
//------------------------------------
const static ofVec2f cCountdownPos(1794, 80);
const static int cCountTime = 60;

//------------------------------------
//System Caller
//------------------------------------
const static string cCmd_MixVideo = "mixVideo.bat ";

//------------------------------------
//Simulator Key
//------------------------------------
const static WORD cStartRocodeKey = VK_F1;
const static WORD cEndRocodeKey = VK_F2;

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

const static string cUPLOAD_URL = "http://events2.artgital.com/711DJ/s/711DJ.php";

//----------------------------
//QR Code Printer
//----------------------------
static const string cSHARE_URL = "http://events2.artgital.com/711DJ/s/share.php?id=";
static const string cQR_IP = "127.0.0.1";
static const int cQR_PORT = 11999;
static const string cQR_START_TEX = "\x01";
static const string cQR_END_TEX = "\x01";
static const string cQR_INTERVAL_TEX = "\x02";

//------------------------------------
//Disk
//------------------------------------
const static float cCoinRadius = 5.0f;
const static float cScannerAndCoinDegree = 180.0f;
