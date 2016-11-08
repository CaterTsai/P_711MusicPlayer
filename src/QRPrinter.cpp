#include "QRPrinter.h"

//---------------------------------------------
void qrPrinter::setup()
{
	if (_udpConnection.Create())
	{
		_udpConnection.Connect(cQR_IP.c_str(), cQR_PORT);
		_udpConnection.SetNonBlocking(true);
		_isSetup = true;
	}
}

//---------------------------------------------
void qrPrinter::printQRCode(string id)
{
	if (_isSetup)
	{
		string url_ = getUrl(id);
		_udpConnection.Send(url_.c_str(), url_.length());
	}
}

//---------------------------------------------
string qrPrinter::getUrl(string id)
{
	return cQR_START_TEX + cSHARE_URL + id + cQR_INTERVAL_TEX + cQR_END_TEX;
}

//--------------------------------------------------------------
string qrPrinter::ws2s(const wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, 0);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, 0);
	return strTo;
}