#ifndef _OFX_EFFECTS_SOUND_PLAYER_
#define _OFX_EFFECTS_SOUND_PLAYER_

#include "ofMain.h"
#include "math.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"

enum eROLL_TYPE
{
	eNONE_ROLL,
	eROLL_BEAT_ONE_THIRTYTHREE, // 1/32
	eROLL_BEAT_ONE_SIXTEEN, // 1/16
	eROLL_BEAT_ONE_EIGHT,	// 1/8
	eROLL_BEAT_ONE_QUARTER, // 1/4
	eROLL_BEAT_ONE_HALF,	// 1/2
	eROLL_BEAT_ONE,			// 1/1
	eROLL_BEAT_TWO,			// 2/1
};

struct stROLL_CHEAT
{
	eROLL_TYPE				eRollType;
	unsigned long long		uint64CheatTime;
};

class ofxEffectsSoundPlayer
{
public:
	ofxEffectsSoundPlayer():m_bIsSetup(false), m_bIsLoadSuccess(false){};
	void Setup();
	void Update();
	void Exit();
	
	//Control
	bool LoadSounds(std::string strFilePath, bool bLoop, float fBPM);
	void Play();
	void Pause();
	void Stop();
	unsigned int GetPosition();
	float GetPercentagePosition();
	void SetPosition(unsigned int uiPosition);
	void SetVolume(float fVol);

	//
	bool IsPlaying();
	bool IsPauseing();

	//Effectes
	void ApplyReverb();
	void SetHighPass(bool bApply, float fCutOffHz);
	void SetHighPass(bool bApply);
	void SetLowPass(bool bApply, float fCutOffHz);
	void SetLowPass(bool bApply);
	void SetDistortion(bool bApply);
	void SetDistortion(bool bApply, float fLevel); // 0.0~1.0
	void SetPitchShift(bool bApply);
	void SetPitchShift(bool bApply, float fPitchScale); // 0.5~2.0 (0.5:one octave down, 2.0:one octave up)
	
	void SetRoll(bool bApply);
	void SetRoll(bool bApply, eROLL_TYPE eRollType);
	
	bool IsHighPassActive();
	bool IsDistortionActive();
	bool IsPitchShiftActive();
	bool IsRollActive();
	
	void SetCheat(bool bEnableCheat);

private:
	void SetupRoll();
	unsigned long long GetRollMicroTime(eROLL_TYPE eRollType);
	void ErrorCheck();

private:
	FMOD::System			*m_oSys;
	FMOD::Sound				*m_oSound;
	FMOD::Channel			*m_oChannel;
	FMOD::DSP				*m_oDSPHighPass;
	FMOD::DSP				*m_oDSPLowPass;
	FMOD::DSP				*m_oDSPDistortion;
	FMOD::DSP				*m_oDSPPitchShift;

	FMOD_RESULT				m_oResult;
	bool					m_bIsSetup, m_bIsLoadSuccess;

	//For Roll
	eROLL_TYPE				m_eNowRollType;
	unsigned long long		m_uint64MicroTimer;
	
	std::map<eROLL_TYPE, double>	m_oRollTimeSet;


	unsigned int			m_uintRollTriggerSoundTimeMS;	//Roll Trigger
	unsigned int			m_uintRollStartTimeMS;			//Roll Start Time

	unsigned long long		m_uint64RollTriggerSysTimeMS;
	unsigned long long		m_uint64RollMicroTime;

	bool						m_bIsRoll;
	double					m_fMSPerBeat;
	double					m_fMSHalfBeat;
	double					m_fMSOneQuarteBeat;

	double					m_fMicroSPerBeat;
	double					m_fMicroSHalfBeat;
	double					m_fMicroSOneQuarteBeat;

	unsigned int			m_uintSoundLength;

	//Main Timer
	unsigned long long		m_uint64MainTimer;

	//Cheat Mode
	bool								m_bEnableCheatMode;
	stROLL_CHEAT			m_stCheatList[11];
	int								m_iCheatIndex;
};

//--------------------------------------------------------------

typedef struct
{
	FMOD::Sound*	pSound;
	FMOD::Channel*	pChannel;
	std::string		strID;
}stEFFECTS_AUDIO;


typedef std::map<std::string, stEFFECTS_AUDIO>	EFFECTS_AUDIO_MAP;
typedef EFFECTS_AUDIO_MAP::iterator					EFFECTS_AUDIO_ITER;

class ofxEffectsAudioPlayer
{
public:
	ofxEffectsAudioPlayer():m_bIsSetup(false){};
	void Setup();
	void Update();
	void Exit();

	void AddAudio(std::string strFilePath, std::string strID, bool bIsLoop);
	void PlayAudio(std::string strID, float fVolume = 1.0, unsigned int uiPosition = 0.0);
	void StopAudio(std::string strID);
	void StopAllAudio();
	void SetVolume(std::string strID, float fVolume);
	void ApplyReverb();

	void SetPosition(std::string strID, unsigned int uiPosition);

private:
	void ErrorCheck();

private:
	FMOD::System			*m_oSys;

	FMOD_RESULT				m_oResult;
	bool					m_bIsSetup;

	EFFECTS_AUDIO_MAP		m_oAudioMap;
};

#endif //_OFX_EFFECTS_SOUND_PLAYER_