#include "visionDisplay.h"

#pragma region Basic Method
//--------------------------------------------------------------
void visionDisplay::setup(ofVec2f center, int width, int height)
{
	setDrawArea(center, width, height);
}

//--------------------------------------------------------------
void visionDisplay::update(float delta)
{
	updateBasic();
	updateAllPlayer(delta);
	updateFader(delta);
}

//--------------------------------------------------------------
void visionDisplay::draw()
{	
	drawBasic();
	drawAllPlayer(_drawArea.getCenter().x, _drawArea.getCenter().y, _drawArea.getWidth(), _drawArea.getHeight());
	drawFader();
}

//--------------------------------------------------------------
bool visionDisplay::isPlaying()
{
	return _isVisionStart;
}

//--------------------------------------------------------------
void visionDisplay::play()
{
	_isVisionStart = true;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->play();
	}

	_basicPlayer.play();

	fadein();
}

//--------------------------------------------------------------
void visionDisplay::stop()
{
	_isVisionStart = false;
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->stop();
	}

	_basicPlayer.stop();
	_basicPlayer.setFrame(0);
	_basicPlayer.update();
}


//--------------------------------------------------------------
void visionDisplay::setDrawArea(ofVec2f center, int width, int height)
{
	_drawArea.setFromCenter(center, width, height);
}
#pragma endregion

#pragma region Fader
//--------------------------------------------------------------
void visionDisplay::fadeout()
{
	_animFadeAlpha.setDuration(cCoinFadeoutTime);
	_animFadeAlpha.animateFromTo(0, 255);
}

//--------------------------------------------------------------
void visionDisplay::fadein()
{
	_animFadeAlpha.setDuration(cCoinFadeinTime);
	_animFadeAlpha.animateFromTo(255, 0);
}

//--------------------------------------------------------------
void visionDisplay::updateFader(float delta)
{
	if (_animFadeAlpha.isAnimating())
	{
		_animFadeAlpha.update(delta);
	}
}

//--------------------------------------------------------------
void visionDisplay::drawFader()
{
	ofPushStyle();
	{
		ofFill();
		ofSetColor(255, _animFadeAlpha.getCurrentValue());
		ofRect(_drawArea);
	}
	ofPopStyle();
}

#pragma endregion

#pragma region Basic Video
//--------------------------------------------------------------
void visionDisplay::updateBasic()
{
	if (_basicPlayer.isLoaded() && _basicPlayer.isPlaying())
	{
		_basicPlayer.update();
	}
}

//--------------------------------------------------------------
void visionDisplay::drawBasic()
{
	if (_basicPlayer.isPlaying())
	{
		ofPushStyle();
		ofSetColor(255);
		_basicPlayer.draw(_drawArea);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void visionDisplay::loadBasic(string path)
{
	_basicPlayer.setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer()));
	_basicPlayer.setLoopState(ofLoopType::OF_LOOP_NORMAL);

	_basicPlayer.loadMovie(path);
}

#pragma endregion

#pragma region Vision Player
//--------------------------------------------------------------
void visionDisplay::addPlayer(eAudioType eType, ePlayerType ePlayerType, string path, int level, float extendTime)
{
	if (_playerMgr.find(eType) == _playerMgr.end())
	{
		switch (ePlayerType)
		{
			case eLoopingPlayer:
			{
				_playerMgr.insert(make_pair(eType, ofPtr<loopingVisionPlayer>(new loopingVisionPlayer(level, extendTime))));
				_playerMgr[eType]->load(path);
				break;
			}
			case eTriggerPlayer:
			{
				_playerMgr.insert(make_pair(eType, ofPtr<triggerVisionPlayer>(new triggerVisionPlayer(level))));
				_playerMgr[eType]->load(path, false);
				break;
			}
		}
		

		try
		{
			_drawLevel.at(level) = eType;
		}
		catch (exception e)
		{
			ofLog(OF_LOG_ERROR, "[visionDisplay::addPlayer]Level over!!");
		}
		
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[visionDisplay::addPlayer]This audio type alread exist");
	}
}


//--------------------------------------------------------------
bool visionDisplay::getIsPlaying(eAudioType eType)
{
	try {
		auto& Iter_ = _playerMgr.at(eType);
		return Iter_->isPlaying();
	}
	catch (out_of_range e) {
		ofLog(OF_LOG_ERROR, "[visionDisplay:getIsPlaying]Out of range");
		return eStateUnknow;
	}
}

//--------------------------------------------------------------
void visionDisplay::playerToggle(eAudioType eType)
{
	if (!_isVisionStart)
	{
		return;
	}
	bool isPlaying_ = getIsPlaying(eType);
	if (!isPlaying_)
	{
		playerIn(eType);
	}
	else
	{
		playerOut(eType);
	}
}

//--------------------------------------------------------------
void visionDisplay::playerIn(eAudioType eType)
{
	if (!_isVisionStart)
	{
		return;
	}
	try {
		auto& Iter_ = _playerMgr.at(eType);
		Iter_->in();
	}
	catch (out_of_range e) {
		ofLog(OF_LOG_ERROR, "[visionDisplay:playerIn]Out of range");
	}
}

//--------------------------------------------------------------
void visionDisplay::playerOut(eAudioType eType)
{
	if (!_isVisionStart)
	{
		return;
	}
	try {
		auto& Iter_ = _playerMgr.at(eType);
		Iter_->out();
	}
	catch (out_of_range e) {
		ofLog(OF_LOG_ERROR, "[visionDisplay:playerOut]Out of range");
	}
}

//--------------------------------------------------------------
void visionDisplay::updateAllPlayer(float delta)
{
	for (auto& Iter_ : _playerMgr)
	{
		Iter_.second->update(delta);
	}
}

//--------------------------------------------------------------
void visionDisplay::drawAllPlayer(int x, int y, int width, int height)
{
	//for (auto& Iter_ : _playerMgr)
	//{
	//	Iter_.second->draw(x, y, width, height);
	//}

	for (auto& Iter_ : _drawLevel)
	{
		_playerMgr[Iter_]->draw(x, y, width, height);
	}
}
#pragma endregion

#pragma region Singletion
//--------------------------------------------------------------
visionDisplay* visionDisplay::pInstance = 0;

//--------------------------------------------------------------
visionDisplay* visionDisplay::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new visionDisplay();
	}
	return pInstance;
}

//--------------------------------------------------------------
void visionDisplay::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion