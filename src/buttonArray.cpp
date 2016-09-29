#include "buttonArray.h"

//--------------------------------------------------------------
void buttonArray::setup(int width, int height, int row, int col)
{
	if (!_isSetup)
	{
		int buttonWidth_ = width / col * 0.8;
		int buttonHeight_ = height / row * 0.8;
		int buttonWidthInv_ = (width - buttonWidth_ * col) / (col - 1);
		int buttonHeightInv_ = (height - buttonHeight_ * row) / (row - 1);
				
		for (int rIdx_ = 0; rIdx_ < row; rIdx_++)
		{
			int posY_ = rIdx_ * (buttonHeight_ + buttonHeightInv_);

			for (int cIdx_ = 0; cIdx_ < col; cIdx_++)
			{
				int posX_ = cIdx_ * (buttonWidth_ + buttonWidthInv_);

				ofRectangle buttonRect_(posX_ - width/2, posY_ - height/2, buttonWidth_, buttonHeight_);
				_buttonRect.push_back(buttonRect_);
			}
		}
		_isSetup = true;
	}
}

//--------------------------------------------------------------
void buttonArray::draw(ofVec2f center)
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(center);
	{
		int index_ = 0;
		for (auto& Iter_ : _buttonMgr)
		{
			(Iter_.second) ? ofSetColor(255, 0, 0) : ofSetColor(255);

			ofFill();
			ofRect(_buttonRect[index_]);

			ofNoFill();
			ofSetColor(0);
			ofRect(_buttonRect[index_]);

			index_++;
		}
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void buttonArray::addButton(eButtonType type)
{
	if (_buttonMgr.find(type) == _buttonMgr.end())
	{
		_buttonMgr.insert(make_pair(type, false));
	}

}

//--------------------------------------------------------------
void buttonArray::troggleButton(eButtonType type)
{
	try {
		auto& Iter_ = _buttonMgr.at(type);
		Iter_ = !Iter_;
	}
	catch (out_of_range e)
	{
		ofLog(OF_LOG_ERROR, "[buttonArray:troggleButton]Out of range");
	}
}
