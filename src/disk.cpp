#include "disk.h"

#pragma region Channel
ofEvent<eAudioType> channel::_channelToggle = ofEvent<eAudioType>();
//-----------------------------------------
channel::channel(eAudioType eType, float channelRadius, float coinRadius)
	:_eType(eType)
	, _isTrigger(false)
{
	_channelRadius = channelRadius;
	_coinSize = ( coinRadius / channelRadius) * 360.0;
}

//-----------------------------------------
void channel::update(float scanDegree)
{
	bool triggerEvent_ = checkCoin(scanDegree) ^ _isTrigger;

	if (triggerEvent_)
	{
		_isTrigger = !_isTrigger;
		ofNotifyEvent(_channelToggle, _eType);
	}
}

//-----------------------------------------
void channel::draw()
{
	ofPushStyle();
	
	ofSetColor(255);
	ofCircle(0, 0, _channelRadius);

	ofSetColor(0);
	ofNoFill();
	ofCircle(0, 0, _channelRadius);

	ofSetColor(255);
	for (auto& Iter_ : _coinList)
	{
		ofPath arc_;
		arc_.setArcResolution(20);
		arc_.setStrokeWidth(0);
		arc_.setFillColor(ofColor(255, 0, 0));
		arc_.arc(0, 0, _channelRadius, _channelRadius, Iter_.first, Iter_.second);

		arc_.draw();
	}
	ofPopStyle();
}

//-----------------------------------------
void channel::addCoin(float scanDegree)
{
	
	float coinFrom_ = scanDegree + cScannerAndCoinDegree;
	if (coinFrom_ >= 360.0f)
	{
		coinFrom_ -= 360.0f;
	}
	float coinTo_ = coinFrom_ + _coinSize;

	if (coinTo_ >= 360.0f)
	{
		addCoinToList(coinFrom_, 360.0f);
		addCoinToList(0.0f, coinTo_ - 360.0f);
	}
	else
	{
		
		addCoinToList(coinFrom_, coinTo_);
	}
	
}

//-----------------------------------------
void channel::clearCoin()
{
	_coinList.clear();
}

//-----------------------------------------
void channel::addCoinToList(float coinFrom, float coinTo)
{
	bool alreadyInList_ = false;
	
	//Check is cover
	for (auto& Iter_ : _coinList)
	{
		if (coinFrom < Iter_.first && Iter_.first <= coinTo)
		{
			//Left
			Iter_.first = coinFrom;
			alreadyInList_ = true;
			break;
		}
		else if (coinFrom <= Iter_.second && Iter_.second < coinTo)
		{
			//Right
			Iter_.second = coinTo;
			alreadyInList_ = true;			
			break;
		}
	}

	if (!alreadyInList_)
	{	
		addNewCoin(coinFrom, coinTo);
	}
	else
	{
		//mergeCoinList();
	}
	
}

//-----------------------------------------
bool channel::checkCoin(float degree)
{
	bool inCoinRange_ = false;
	for (auto& Iter_ : _coinList)
	{
		if (degree >= Iter_.first && degree <= Iter_.second)
		{
			inCoinRange_ = true;
			break;
		}
	}

	return inCoinRange_;
}

//-----------------------------------------
void channel::mergeCoinList()
{
	if (_coinList.size() < 2)
	{
		return;
	}

	auto Iter_ = _coinList.begin();
	auto nowIter_ = Iter_;
	nowIter_++;
	for (; nowIter_ != _coinList.end();)
	{
		if (nowIter_->first < Iter_->second)
		{
			Iter_->second = nowIter_->second;
			nowIter_ = _coinList.erase(nowIter_);
		}
		else
		{
			nowIter_++;
			Iter_++;
		}
	}
}

//-----------------------------------------
void channel::addNewCoin(float coinFrom, float coinTo)
{
	if (_coinList.size() == 0)
	{
		_coinList.push_back(make_pair(coinFrom, coinTo));
	}
	else
	{
		auto Iter_ = _coinList.begin();
		for (; Iter_ != _coinList.end(); Iter_++)
		{
			if (Iter_->first > coinFrom)
			{
				break;
			}
		}

		if (Iter_ == _coinList.end())
		{
			_coinList.push_back(make_pair(coinFrom, coinTo));
		}
		else
		{
			_coinList.insert(Iter_, make_pair(coinFrom, coinTo));
			
		}
	}
}
#pragma endregion

#pragma region Disk
void disk::setup(int size)
{
	_diskSize = size;
}

//--------------------------------------------------------------
void disk::update(float delta)
{
	if (_isStart)
	{
		_rotateDegree += _rotateV * delta;
		if (_rotateDegree > 360.0)
		{
			_rotateDegree -= 360.0;
		}
		else if (_rotateDegree < -360.0)
		{
			_rotateDegree += 360.0;
		}

		for (auto& Iter_ : _diskMgr)
		{
			Iter_.second.update(-_rotateDegree);
		}
	}
}

//--------------------------------------------------------------
void disk::draw(ofVec2f center)
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(center);
	ofRotateZ(_rotateDegree);

	//Channal
	for (auto& Iter_ : _diskMgr)
	{
		Iter_.second.draw();
	}

	//Line
	ofSetColor(0);
	ofSetLineWidth(3);
	ofPushMatrix();
	float fRotateUnit_ = 360.0 / 8.0;
	for (int idx_ = 0; idx_ < 8; idx_++)
	{
		ofLine(0, 0, 0, _diskSize / 2.0f);
		ofRotateZ(fRotateUnit_);
	}
	ofPopMatrix();
	
	ofPopMatrix();
	
	//Scanner
	ofPushMatrix();
	ofTranslate(center);

	ofFill();
	ofSetColor(255);
	ofRect(_diskSize * 0.05, -5, _diskSize * 0.5, 10);

	ofNoFill();
	ofSetColor(0);
	ofRect(_diskSize * 0.05, -5, _diskSize * 0.5, 10);

	ofPopMatrix();

	ofPopStyle();
}

//--------------------------------------------------------------
void disk::addChannel(eAudioType type, float channelRadius)
{
	if (_diskMgr.find(type) != _diskMgr.end())
	{
		return;
	}

	channel newChannel_(type, channelRadius, cCoinRadius);
	_diskMgr.insert(make_pair(type, newChannel_));
}

//--------------------------------------------------------------
void disk::startMusic()
{
	_isStart = true;
}

//--------------------------------------------------------------
void disk::stopMusic()
{
	_isStart = false;
}

//--------------------------------------------------------------
void disk::coin(eAudioType type)
{
	try {
		auto& Iter_ = _diskMgr.at(type);
		Iter_.addCoin(-_rotateDegree);
	}
	catch (out_of_range e)
	{
		ofLog(OF_LOG_ERROR, "[disk:channalOn]Out of range");
	}
}

//--------------------------------------------------------------
void disk::clearCoin()
{
	for (auto& Iter_ : _diskMgr)
	{
		Iter_.second.clearCoin();
	}
}

#pragma endregion
