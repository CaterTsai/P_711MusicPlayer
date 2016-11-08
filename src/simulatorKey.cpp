#include "simulatorKey.h"

#pragma region Singletion
//--------------------------------------------------------------
simulatorKey* simulatorKey::pInstance = 0;

//--------------------------------------------------------------
simulatorKey* simulatorKey::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new simulatorKey();
	}
	return pInstance;
}

//--------------------------------------------------------------
void simulatorKey::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion