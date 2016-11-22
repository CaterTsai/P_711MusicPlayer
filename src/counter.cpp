#include "counter.h"

ofEvent <string> counter::_countEvent;

//--------------------------------------------------------------
void counter::load(string iconPath, string fontPath)
{
	ofTrueTypeFont::setGlobalDpi(72);
	_font.loadFont(fontPath, cCounterFontSize, true, true);

	_heart.loadImage(iconPath);
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
		ofSetColor(155, 218, 165);
		ofFill();
		ofRectRounded(rect, 30);

		//Bar
		float percentage_ = _counterTimer / _targetTime;
		float barLength_ = rect.width * percentage_;
		ofSetColor(77, 188, 93);
		ofFill();
		ofRectRounded(rect.position, barLength_, rect.height, 30);

		//Text
		ofSetColor(255);
		if (_font.isLoaded())
		{
			ofVec2f fontPos_ = rect.position;
			string val_ = ofToString(static_cast<int>(percentage_ * 100.0f + 0.5f)) + "%";
			auto fontRect_ = _font.getStringBoundingBox(val_, 0, 0);
			fontRect_.width *= 1.3;

			if (fontRect_.getWidth() < barLength_)
			{
				fontPos_.x += (barLength_ - fontRect_.getWidth());
				fontPos_.y += rect.height * 0.5 + fontRect_.getHeight() / 2;

				_font.drawString(val_, fontPos_.x, fontPos_.y);
			}
		}

		//Heart
		ofSetColor(255);
		ofVec2f heartPos_(rect.position.x - cHeartSize * 0.5 + barLength_, cHeartHight);
		_heart.draw(heartPos_, cHeartSize, cHeartSize);
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


