#include "counter.h"

ofEvent <string> counter::_countEvent;

//--------------------------------------------------------------
void counter::loadFont(string fontPath)
{
	ofTrueTypeFont::setGlobalDpi(72);
	_font.loadFont(fontPath, cCounterFontSize, true, true);
}

//--------------------------------------------------------------
void counter::update(const float delta)
{
	if (!_isStart)
	{
		return;
	}

	_counterTimer += delta;
	updateCounterEvent();
	if (_counterTimer >= _targetTime)
	{
		_isStart = false;
	}

	
}

//--------------------------------------------------------------
void counter::draw(ofRectangle rect)
{
	if (!_isStart)
	{
		return;
	}

	ofPushStyle();
	{
		//Background
		ofSetColor(255);
		ofFill();
		ofRect(rect);

		ofSetColor(43, 170, 63);
		ofNoFill();
		ofSetLineWidth(1.0);
		ofRect(rect);

		//Bar
		float percentage_ = _counterTimer / _targetTime;
		float barLength_ = rect.width * percentage_;
		ofSetColor(43, 170, 63);
		ofFill();
		ofRect(rect.position, barLength_, rect.height);

		//Text
		ofSetColor(255);
		if (_font.isLoaded())
		{
			ofVec2f fontPos_ = rect.position;
			string val_ = ofToString(static_cast<int>(percentage_ * 100.0f + 0.5f)) + "%";
			auto fontRect_ = _font.getStringBoundingBox(val_, 0, 0);
			fontRect_.width *= 1.1;

			if (fontRect_.getWidth() < barLength_)
			{
				fontPos_.x += (barLength_ - fontRect_.getWidth());
				fontPos_.y += rect.height * 0.5 + fontRect_.getHeight() / 2;

				_font.drawString(val_, fontPos_.x, fontPos_.y);
			}
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void counter::start(float time)
{
	if (_isStart)
	{
		return;
	}

	_isStart = true;
	_counterTimer = 0.0;
	_targetTime = time;

	resetCounterEvent();
}

//--------------------------------------------------------------
void counter::stop()
{
	if (!_isStart)
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


