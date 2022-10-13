#pragma once
#include "Base.h"
#include "GameObject.h"
#include "ControlRoom.h"


class CControlRoomMgr : public CBase
{
	DECLARE_SINGLETON(CControlRoomMgr)

private:
	explicit CControlRoomMgr();
	virtual ~CControlRoomMgr();

public:
	
	list<CGameObject*>&		Get_ControlRoomList() { return m_ControlRoomList; }
	void					Add_ControlRoomList(CGameObject* pControlRoom) { m_ControlRoomList.push_back(pControlRoom); }


	

private:
	virtual void Free(void);
	list<CGameObject*>  m_ControlRoomList;

};

