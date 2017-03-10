#include "serialCtrl.h"

#pragma region serialCtrl
//--------------------------------------------
void serialCtrl::setup(string port, int baud)
{
	_isSetup = _serial.setup(port, baud);
}

//--------------------------------------------
void serialCtrl::update()
{
	if (!_isSetup)
	{
		return;
	}

	read();
	sendVal();
	if (_msgComplete)
	{
		decodeMsg();
		_msgComplete = false;
	}
}

//--------------------------------------------
void serialCtrl::send(unsigned char val)
{
	if (!_isSetup)
	{
		return;
	}

	_needSend = true;
	_val = val;
}

//--------------------------------------------
void serialCtrl::read()
{
	_msgComplete = false;
	int nRead_ = 0;
	unsigned char serialRead_[cSerialReadNum];
	ZeroMemory(serialRead_, sizeof(unsigned char) * cSerialReadNum);
	_msg.clear();

	while (nRead_ = _serial.readBytes(serialRead_, cSerialReadNum) > 0)
	{
	}

	string tempMsg_((char*)serialRead_);
	int sta_ = tempMsg_.find_last_of(cSerialMsgSTX);
	int end_ = tempMsg_.find_last_of(cSerialMsgEND);

	if (end_ != string::npos && sta_ > end_)
	{
		tempMsg_ = tempMsg_.substr(0, end_);
		sta_ = tempMsg_.find_last_of(cSerialMsgSTX);
	}

	if (sta_ == string::npos || end_ == string::npos)
	{
		return;
	}
	else
	{
		_msg = tempMsg_.substr(sta_ + 1, end_ - (sta_ + 1));
		_msgComplete = true;
	}
}

//--------------------------------------------
void serialCtrl::sendVal()
{
	if (!_isSetup || !_needSend)
	{
		return;
	}

	unsigned char msg_[2] = { 0 };
	msg_[0] = _val;
	msg_[1] = '\n';

	_serial.writeBytes(msg_, 2);
	_needSend = false;
}

//--------------------------------------------
void serialCtrl::decodeMsg()
{}
#pragma endregion

#pragma region deviceCtrl
//--------------------------------------------
void deviceCtrl::open()
{
	send('o');
	_isOpen = true;
}

//--------------------------------------------
void deviceCtrl::close()
{
	send('c');
	_isOpen = false;
}

//--------------------------------------------
bool deviceCtrl::isOpen()
{
	return _isOpen;
}

//--------------------------------------------
void deviceCtrl::decodeMsg()
{
	switch (_msg[0])
	{
		case '0':
		{
			//Slider
			//sliderEvent();
			break;
		}
		case '1':
		{
			//Button			
			buttonEvent();
			break;
		}
		default:
		{
			ofLog(OF_LOG_ERROR, "[deviceCtrl::decodeMsg]Unknow event type :" + _msg[0]);
			break;
		}
	}
}

//--------------------------------------------
void deviceCtrl::buttonEvent()
{
	if (_msg.length() < cBtnNum + 2 || _msg.length() > 9)
	{
		return;
	}

	try {
		for (int idx_ = 0; idx_ < cBtnNum; idx_++)
		{
			bool isTrigger_ = (bool)(_msg[2 + idx_] - '0');
			if (_isTrigger[idx_] != isTrigger_)
			{
				serialArgs<bool> args_(_msg[0], idx_, isTrigger_);
				ofNotifyEvent(_buttonEvent, args_);
				_isTrigger[idx_] = isTrigger_;
			}
		}
	}
	catch (exception e)
	{
		ofLog(OF_LOG_ERROR, e.what());
	}
	
}

//--------------------------------------------
void deviceCtrl::sliderEvent()
{
	if (_msg.length() < 6)
	{
		return;
	}
	string val_ = _msg.substr(2, 4);
	float vol_ = (3.0f - atof(val_.c_str()));

	serialArgs<float> args_(_msg[0], _msg[1], vol_);
	ofNotifyEvent(_sliderEvent, args_);
}
#pragma endregion

#pragma region sensorCtrl
//--------------------------------------------
void sensorCtrl::reset()
{
	//Reset
	send('r');
}
//--------------------------------------------
void sensorCtrl::decodeMsg()
{
	switch (_msg[0])
	{
	case '2':
	{
		//Color sensor
		if (_msg.size() == 7)
		{
			sensorEvent();
		}
		else
		{
			ofLog(OF_LOG_ERROR, "[sensorCtrl::decodeMsg]Msg length wrong : " +ofToString(_msg.size()));
		}
		
		break;
	}
	default:
	{
		ofLog(OF_LOG_ERROR, "[sensorCtrl::decodeMsg]Unknow event type :" + _msg[0]);
		break;
	}
	}
}

//--------------------------------------------
void sensorCtrl::sensorEvent()
{
	int sensorNo_ = (int)(_msg[1] - '0');
	if (sensorNo_ < 0 || sensorNo_ >= cCoinSetNum)
	{
		return;
	}


	for (int idx_ = 0; idx_ < cCoinNumEachSet; idx_++)
	{
		int index_ = idx_ + sensorNo_ * cCoinNumEachSet;
		bool sensorTrigger_ = (bool)(_msg[2 + idx_] - '0');
		if (_isTrigger[index_] != sensorTrigger_)
		{
			serialArgs<bool> args_(_msg[0], index_, sensorTrigger_);
			ofNotifyEvent(_sensorEvent, args_);
			_isTrigger[index_] = sensorTrigger_;
		}
	}
}
#pragma endregion


