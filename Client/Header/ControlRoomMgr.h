#pragma once
#include "Base.h"
#include "GameObject.h"

class CControlRoomMgr : public CBase
{
	DECLARE_SINGLETON(CControlRoomMgr)

private:
	explicit CControlRoomMgr();
	virtual ~CControlRoomMgr();

public:
	
private:
	virtual void Free(void);


};

