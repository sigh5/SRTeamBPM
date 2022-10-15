#pragma once
#include "Base.h"

#include "Engine_Include.h"

class CMouseMgr :public CBase
{
	DECLARE_SINGLETON(CMouseMgr)
private:
	explicit CMouseMgr();
	virtual ~CMouseMgr();


public:
	void Mouse_Change(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* pFilepath);

	void  SendMessage_test();


public:
	void		Set_Mouse(_bool bEvent) { m_bMouse = bEvent; }
	const _bool&	Get_Mouse() { return m_bMouse; }

private:
	_bool				 m_bMouse=false;

	LPDIRECT3DTEXTURE9      m_pCursortex;
	LPDIRECT3DSURFACE9    m_Surfcursor;


		
private:
	virtual void Free(void);

};

