#pragma once

#include "constParameter.h"

class buttonArray
{
public:
	buttonArray()
		:_isSetup(false)
	{}
	void setup(int width, int height, int row, int col);
	void draw(ofVec2f center);

	void addButton(eButtonType type);
	void troggleButton(eButtonType type);


private:
	bool _isSetup;
	vector<ofRectangle> _buttonRect;
	map<eButtonType, bool> _buttonMgr;
};