#pragma once

#include "effectPlayer.h"
#include "effectDSP.h"
//------------------------------------
//ENUM
//------------------------------------
enum eAudioType
{
	eLeft_Crash = 0
	,eLeft_FXPad1
	,eLeft_FXPad2
	,eLeft_Jingo
	,eLeft_Toms
	,eLeft_Max

	,eRight_Bass
	,eRight_Drum
	,eRight_GT
	,eRight_LeadSynth
	,eRight_Synth
	,eRight_Max
};



//------------------------------------
//CLASS
//------------------------------------
class effectMusicMgr
{
public:
	void setup();
	void update(float delta);
	bool isPlaying();
private:
	bool _isPlaying;
#pragma region Basic Music
//-------------------
//Basic Music
//-------------------
public:
	void loadBasic(string path);
	void playBasic();
	void stopBasic();
private:
	ofSoundPlayer	_basicPlayer;
#pragma endregion

#pragma region Effect Player
//-------------------
//EffectPlayer
//-------------------
public:
	void addPlayer(eAudioType eType, string path);
	void play();
	void stop();

	eEffectFadeState getPlayerState(eAudioType eType);
	void playerIn(eAudioType eType);
	void playerOut(eAudioType eType);

private:
	void updateAllPlayer(float fDelta);
private:
	map<eAudioType, effectPlayer> _playerMgr;
#pragma endregion

#pragma region Effect
//-------------------
//DSP
//-------------------
private:
	void setupEffect();
	void updateEffect(float delta);

public:
	void activeEffect(eEffectType eType);
	void unactiveEffect(eEffectType eType);
	void troggleEffect(eEffectType eType);

private:
	map<eEffectType, ofPtr<effectDSP>> _effectMgr;

#pragma endregion

#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	effectMusicMgr()
		:_isPlaying(false)
	{};
	~effectMusicMgr()
	{
		effectMusicMgr::Destroy();
	}
	effectMusicMgr(effectMusicMgr const&) {};
	void operator=(effectMusicMgr const&) {};


public:
	static effectMusicMgr* GetInstance();
	static void Destroy();

private:
	static effectMusicMgr *pInstance;
#pragma endregion



};