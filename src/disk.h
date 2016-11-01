#pragma once

#include "constParameter.h"
#include "effectMusicMgr.h"

class channel
{
public:
	channel(eAudioType eType, float channelRadius, float coinRadius);

	void update(float scanDegree);
	void draw();

	void addCoin(float scanDegree);
	void clearCoin();

private:
	void addCoinToList(float coinFrom, float coinTo);
	bool checkCoin(float degree);

	void mergeCoinList();
	void addNewCoin(float coinFrom, float coinTo);

		
private:
	eAudioType	_eType;
	bool _isTrigger;
	
	float _coinSize;
	float _channelRadius;

	// pair<Start Degree, End Degree>, End degree > start degree
	list<pair<float, float>> _coinList; 

//----------------
//event
public:
	static ofEvent<eAudioType>	_channelToggle;
};

class disk
{
private:

public:
	disk()
		:_isStart(false)
		, _rotateV(-36)
		, _rotateDegree(0.0)
	{}
	void setup(int size);
	void update(float delta);
	void draw(ofVec2f center);

	void addChannel(eAudioType type, float channelRadius);

	void startMusic();
	void stopMusic();

	void coin(eAudioType type);
	void clearCoin();


private:
	bool _isStart;
	int _diskSize;
	float _rotateV, _rotateDegree;
	map<eAudioType, channel> _diskMgr;
};