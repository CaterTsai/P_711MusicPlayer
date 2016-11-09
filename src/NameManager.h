#ifndef _NAME_MANAGER_
#define _NAME_MANAGER_

#include <string>
using namespace std;

#define CREATE_VALUE(NAME) static const string NAME = #NAME;
#define CREATE_VALUE_WITH_VALUE(NAME, VALUE) static const string NAME = VALUE;

namespace NAME_MGR
{
	//Counter Event
	CREATE_VALUE(CEvent_ShowHint);
	CREATE_VALUE(CEvent_HideHint);
	CREATE_VALUE(CEvent_StartRecode);
	CREATE_VALUE(CEvent_EndRecode);
	CREATE_VALUE(CEvent_StartFadeout);
	CREATE_VALUE(CEvent_StartEnding);
	CREATE_VALUE(CEvent_CounterFinish);

	//System Caller Event
	CREATE_VALUE(SCEvent_MixVideo);

};



#endif //_NAME_MANAGER_