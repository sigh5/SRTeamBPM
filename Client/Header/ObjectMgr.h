#pragma once
#include "Base.h"
#include "GameObject.h"



class CObjectMgr :public CBase
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	explicit CObjectMgr();
	virtual ~CObjectMgr();

public:
	

public:
	void				Collect_Obj(CGameObject* pObj);		// 사용을 마친 오브젝트를 회수하는 함수
	CGameObject*		Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos);	// 현재 풀에 있는 오브젝트를 재사용하는 함수

	void				Collect_UIObj(CGameObject* pObj);		// 사용을 마친 오브젝트를 회수하는 함수
	CGameObject*		Reuse_MetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev,_float fPosX, _float fPosY, _float fSpeed, int iTexIndex);	// 현재 풀에 있

private:
	list<CGameObject*>		m_BulletList;
	list<CGameObject*>		m_UIMetroList;



	_int					m_iCount;
	_int					m_iTest = -1;
private:
	virtual void Free(void);
};

