#include "disk.h"

void disk::setup(int size)
{
	_maxSize = size;
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
	}
}

//--------------------------------------------------------------
void disk::draw(ofVec2f center)
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(center);
	ofRotateZ(_rotateDegree);
	float radius_ = _maxSize / 2.0f;

	//Channal
	for (auto& Iter_ : _diskMgr)
	{
		(Iter_.second) ? ofSetColor(255, 0, 0) : ofSetColor(255);
		
		ofFill();
		ofCircle(0, 0, radius_);

		ofNoFill();
		ofSetColor(0);
		ofCircle(0, 0, radius_);

		radius_ -= _channalSize;
	}

	//Line
	ofSetColor(0);
	ofSetLineWidth(3);
	ofPushMatrix();
	float fRotateUnit_ = 360.0 / 8.0;
	for (int idx_ = 0; idx_ < 8; idx_++)
	{
		ofLine(0, 0, 0, _maxSize / 2.0f);
		ofRotateZ(fRotateUnit_);
	}
	ofPopMatrix();

	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void disk::addChannal(eAudioType type)
{
	if (_diskMgr.find(type) == _diskMgr.end())
	{
		_diskMgr.insert(make_pair(type, false));

		updateChannalSize();
	}
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
void disk::channalOn(eAudioType type)
{
	try {
		auto& Iter_ = _diskMgr.at(type);
		Iter_ = true;
	}
	catch (out_of_range e)
	{
		ofLog(OF_LOG_ERROR, "[disk:channalOn]Out of range");
	}
}

//--------------------------------------------------------------
void disk::channalOff(eAudioType type)
{
	try {
		auto& Iter_ = _diskMgr.at(type);
		Iter_ = false;
	}
	catch (out_of_range e)
	{
		ofLog(OF_LOG_ERROR, "[disk:channalOff]Out of range");
	}
}

//--------------------------------------------------------------
void disk::updateChannalSize()
{
	_channalSize = _maxSize / 2.0f / _diskMgr.size();
}




