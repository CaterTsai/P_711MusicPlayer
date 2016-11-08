#include "counter.h"

ofEvent <string> counter::_countEvent;
//--------------------------------------------------------------
void counter::setup(const string numberfolder)
{
	if (_isSetup)
	{
		ofLog(OF_LOG_ERROR, "[counter::setup]Already setup");
	}

	_numImgList.resize(10);

	ofDirectory dir_(numberfolder);
	dir_.allowExt("png");
	dir_.listDir();

	if (dir_.numFiles() == 10)
	{
		for (int idx_ = 0; idx_ < dir_.numFiles(); ++idx_)
		{
			ofImage img_;
			img_.loadImage(dir_.getPath(idx_));
			_numImgList[idx_] = img_;

			_numWidth = MAX(img_.getWidth(), _numWidth);
			_numHeight = MAX(img_.getHeight(), _numHeight);
		}
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[counter::setup]Load number failed");
		return;
	}
	_isSetup = true;
}

//--------------------------------------------------------------
void counter::update(const float delta)
{
	if (!_isSetup || !_isStart)
	{
		return;
	}

	_counterTimer -= delta;
	updateCounterEvent();
	if (_counterTimer <= 0.0f)
	{
		_isStart = false;
	}

	
}

//--------------------------------------------------------------
void counter::draw(ofVec2f pos)
{
	if (!_isSetup || !_isStart)
	{
		return;
	}

	int integerTime_ = static_cast<int>(ceil(_counterTimer)) % 100; //0~99
	int tens_ = static_cast<int>(integerTime_ / 10.0f);
	int ones_ = integerTime_ % 10;
	
	if (tens_ < 0 || tens_ >= 10 || ones_ < 0 || ones_ >= 10)
	{
		ofLog(OF_LOG_ERROR, "[counter::draw]Out of range. Tens:%d Ones:%d", tens_, ones_);
		return;
	}

	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(pos);
	{
		
		if (tens_ != 0)
		{
			//TENS
			_numImgList[tens_].draw(-_numWidth, _numImgList[tens_].getHeight() * -0.5);

			//ONES
			_numImgList[ones_].draw(0, _numImgList[ones_].getHeight() * -0.5);
		}
		else
		{
			//ONES
			_numImgList[ones_].draw(_numImgList[ones_].getWidth() * -0.5, _numImgList[ones_].getHeight() * -0.5);
		}
		
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void counter::start(float time)
{
	if (!_isSetup || _isStart)
	{
		return;
	}

	_isStart = true;
	_counterTimer = time;
	_targetTime = time;

	resetCounterEvent();
}

//--------------------------------------------------------------
void counter::stop()
{
	if (!_isSetup || !_isStart)
	{
		return;
	}

	_isStart = true;
	_counterTimer = _targetTime = 0.0f;

	resetCounterEvent();
}

#pragma region Counter Event
//--------------------------------------------------------------
void counter::addCounterEvent(float time, string msg)
{
	_counterEvent.push_back(counterEvent(time, msg));
}

//--------------------------------------------------------------
void counter::updateCounterEvent()
{
	for (auto& Iter_ : _counterEvent)
	{
		if (Iter_.check(_counterTimer))
		{
			string msg_ = Iter_.getMsg();
			ofNotifyEvent(_countEvent, msg_, this);
		}
	}
}

//--------------------------------------------------------------
void counter::resetCounterEvent()
{
	for (auto& Iter_ : _counterEvent)
	{
		Iter_.reset();
	}
}
#pragma endregion


