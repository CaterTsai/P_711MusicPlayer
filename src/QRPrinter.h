#pragma once

#include "constParameter.h"
#include "ofxNetwork.h"

class qrPrinter
{
public:
	qrPrinter()
		:_isSetup(false)
	{}
	void setup();
	void printQRCode(string id);

private:
	string getUrl(string id);
	string ws2s(const wstring& wstr);

private:
	bool _isSetup;
	ofxUDPManager	_udpConnection;
};