#include "ofxCTSystemCaller.h"

//--------------------------------------------------------------
//@CLASS ofxCTSystemCaller
//--------------------------------------------------------------
void ofxCTSystemCaller::addCMD(string strCmd, string name)
{
	systemCmd newCMD_(name, strCmd);

	_CmdQueue.push(newCMD_);
}

//--------------------------------------------------------------
void ofxCTSystemCaller::signal()
{
	_Condition.signal();
}

//--------------------------------------------------------------
void ofxCTSystemCaller::threadedFunction()
{
	while(isThreadRunning())
	{
		if(!_CmdQueue.empty())
		{
			systemCmd Cmd_ = _CmdQueue.front();
			system(Cmd_.getCMD().c_str());
			_CmdQueue.pop();

			string msg_ = Cmd_.getName();
			ofNotifyEvent(SystemCallerFinishEvent, msg_, this);
		}
		else
		{
			_Condition.wait(_Mutex);
		}
	}
}