#include "configData.h"


//--------------------------------------------------------------
config* config::pInstance = 0;

//--------------------------------------------------------------
config::config()
	:audioPath("audiosTPE/")
{}

//--------------------------------------------------------------
config* config::GetInstance()
{
	if (pInstance == 0)
	{
		pInstance = new config();
	}
	return pInstance;
}

//--------------------------------------------------------------
void config::Destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}