#pragma once

#include "effectPlayer.h"
#include "effectDSP.h"
#include "constParameter.h"

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
	bool _isEffectMusicStart;
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
	void addPlayer(eAudioType eType, eAudioGroup eGroup, ePlayerType ePType, string path);
	void play();
	void stop();

	bool getIsPlaying(eAudioType eType);
	void playerToggle(eAudioType eType);
	void playerIn(eAudioType eType);
	void playerOut(eAudioType eType);

	void setGroupBalance(float val);
	void setGroupVol(eAudioGroup _aGroup, float _vol);

private:
	void updateAllPlayer(float fDelta);
	void addToGroupSet(eAudioType eType, eAudioGroup eGroup);
	void updateGroupMaxVol(eAudioGroup eGroup);
private:

	float _groupBlance; // 0.0 : 100% Left, 1.0 100% Right
	map<eAudioType, ofPtr<basicPlayer>> _playerMgr;
	map<eAudioGroup, vector<eAudioType>> _groupSet;
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
		:_isEffectMusicStart(false)
		, _groupBlance(0.5f)
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