#include "videoMgr.h"

videoMgr::videoMgr()
	:_isPlay(false)
{
	_drawArea.set(0, 0, cWindowWidth, cWindowHeight);
}

//----------------------------------------------------------
videoMgr::~videoMgr()
{
	for (auto& Iter_ : _videoSet)
	{
		Iter_.second->closeMovie();
	}
}

//----------------------------------------------------------
void videoMgr::update(float delta)
{
	if (!_isPlay)
	{
		return;
	}


	_videoPlay->update();

	if (_checkFinish && _videoPlay->getIsMovieDone())
	{
		_checkFinish = false;
		ofNotifyEvent(_onVideoDone, _ePlayType);
	}
}

//----------------------------------------------------------
void videoMgr::draw()
{
	if (!_isPlay)
	{
		return;
	}

	ofPushStyle();
	ofSetColor(255);
	{
		_videoPlay->draw(_drawArea);
	}
	ofPopStyle();
	
}

//----------------------------------------------------------
void videoMgr::play(eVideoType type, bool isLoop)
{
	if (_isPlay)
	{
		stop();
	}

	auto& Iter_ = _videoSet.find(type);
	if (Iter_ != _videoSet.end())
	{
		_videoPlay = Iter_->second;
		isLoop ? _videoPlay->setLoopState(ofLoopType::OF_LOOP_NORMAL):_videoPlay->setLoopState(ofLoopType::OF_LOOP_NONE);
		
		_videoPlay->play();
		_videoPlay->setFrame(0);
		

		_ePlayType = type;
		_isPlay = true;
		_checkFinish = !isLoop;
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[videoMgr::play]Can't found video type :" + type);
	}
}

//----------------------------------------------------------
void videoMgr::stop()
{
	if (!_isPlay)
	{
		return;
	}

	_videoPlay->stop();
	_isPlay = false;
}

//----------------------------------------------------------
void videoMgr::addVideo(eVideoType type, string path)
{
	if (_videoSet.find(type) != _videoSet.end())
	{
		ofLog(OF_LOG_ERROR, "[videoMgr::addVideo]add video failed");
		return;
	}

	_videoSet.insert(make_pair(type, ofPtr<ofVideoPlayer>(new ofVideoPlayer())));
	_videoSet[type]->setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer()));
	if (!_videoSet[type]->loadMovie(path))
	{
		ofLog(OF_LOG_ERROR, "[videoMgr::addVideo]load video failed :" + path);
		_videoSet.erase(type);
	}
}

//----------------------------------------------------------
void videoMgr::setDrawArea(ofVec2f center, int width, int height)
{
	_drawArea.setFromCenter(center, width, height);
}
