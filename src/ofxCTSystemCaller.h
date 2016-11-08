#pragma once
#include "ofMain.h"


class ofxCTSystemCaller : ofThread
{
////////////////////////////
//CLASS systemCmd
////////////////////////////
private:
	class systemCmd
	{
	public:
		systemCmd(string cmdName, string cmd)
			:name(cmdName)
			,strCMD(cmd)
		{}

		string getName()
		{
			return name;
		}

		string getCMD()
		{
			return strCMD;
		}
	private:
		string name;
		string strCMD;
	};


//------------------------------------
public:	
	void addCMD(string strCmd, string name);
	void signal();
	void threadedFunction();
	

///////////////////////
//Inline method
///////////////////////
	inline void start()
	{
		startThread(true, false);
	}

	inline void stop()
	{
		stopThread();
	}
	
private:
	queue<systemCmd>		_CmdQueue;
	ofMutex					_Mutex;
	Poco::Condition			_Condition;

////////////////////////////
//Event
////////////////////////////
public:
	ofEvent<string>	SystemCallerFinishEvent;


};
