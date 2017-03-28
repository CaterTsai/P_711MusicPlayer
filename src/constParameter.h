#pragma once


//#define _USE_SIMULATION_
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
	, eCenter_Button_4
	, eCenter_Button_5
	, eCenter_Button_6

	, eAudioNum
};

enum eAudioGroup
{
	eAudioGroup_Left = 0
	,eAudioGroup_Right
	,eAudioGroup_Button

};

enum ePlayerState
{
	ePlayerWait = 0
	,ePlayerPlaying
	,ePlayerEnding
	,ePlayerFadeout
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
	, eButton_Audio_4
	, eButton_Audio_5
	, eButton_Audio_6

};

enum eSliderType {
	eSliderLeft = 0
	,eSliderRight
	,eSliderCenter
};

enum eVideoType
{
	eVideoTutorial	= 0
	,eVideoHints
	,eVideoUploading
	,eVideoEnding
};

const static int cWindowWidth = 1920;
const static int cWindowHeight = 1080;

const static int cCountTime = 90;

const static float cCoinFadeinTime = 1.0;
const static float cCoinFadeoutTime = 3.0;
const static float cTriggerCoinFadeout = cCountTime - cCoinFadeoutTime;
const static float cEndingTime = cCountTime - 0.;
//Video
const static string cVideoTempPath = "data/recodeTemp/";
const static float cRecodeLength = 22.0f;
const static float cStartRecode = cCountTime - cRecodeLength;
const static float cEndRecode = cCountTime;

const static float cShowHits = 20.0f;
const static float cShowHightLength = 5.0f;
const static float cHideHits = cShowHits + cShowHightLength;


//------------------------------------
//Counter
//------------------------------------
const static int cCounterFontSize = 36;
const static ofRectangle cCounterRect(285, 1036, 1353, 35);
const static int cHeartHight = 955;
const static int cHeartSize = 97;

//------------------------------------
//System Caller
//------------------------------------
const static string cCmd_MixVideo = "mixVideo.bat ";
const static string cCmd_ClearVideo = "removeVideo.bat";

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
//const static string cUPLOAD_URL = "http://127.0.0.1/711DJ/s/711DJ.php";

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
const static float cScannerAndCoinDegree = 20.0f;

