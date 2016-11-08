#pragma once

#include "constParameter.h"

class simulatorKey
{
public:
	void sendKey(WORD key)
	{
		_ip.ki.wVk = key;
		_ip.ki.dwFlags = 0;

		SendInput(1, &_ip, sizeof(INPUT));

		_ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &_ip, sizeof(INPUT));
	}

private:
	INPUT _ip;

#pragma region Singleton
//-------------------
//Singleton
//-------------------
private:
	simulatorKey()
	{
		_ip.type = INPUT_KEYBOARD;
		_ip.ki.wScan = 0;
		_ip.ki.time = 0;
		_ip.ki.dwExtraInfo = 0;
	};
	~simulatorKey()
	{
		simulatorKey::Destroy();
	}
	simulatorKey(simulatorKey const&) {};
	void operator=(simulatorKey const&) {};

public:
	static simulatorKey* GetInstance();
	static void Destroy();

private:
	static simulatorKey *pInstance;
#pragma endregion
};