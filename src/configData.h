#pragma once

#include "constParameter.h"

class config
{
public:
	const string audioPath;
	
private:
	config();
	~config(){}
	config(config const&) {};
	void operator=(config const&) {};

public:
	static config* GetInstance();
	static void Destroy();

private:
	static config *pInstance;
};
