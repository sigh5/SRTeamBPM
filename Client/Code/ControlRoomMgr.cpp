#include "stdafx.h"
#include "ControlRoomMgr.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CControlRoomMgr)


CControlRoomMgr::CControlRoomMgr()
{
}


CControlRoomMgr::~CControlRoomMgr()
{
	Free();
}

void CControlRoomMgr::Free(void)
{
}
