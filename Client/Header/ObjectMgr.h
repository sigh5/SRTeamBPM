#pragma once
#include "Base.h"
#include "GameObject.h"

#include "BaseBullet.h"
class CGhul;
class CObjectMgr :public CBase
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	explicit CObjectMgr();
	virtual ~CObjectMgr();

public:
	

public:
	void				Collect_MonsterBulletObj(CBaseBullet* pObj);		// 사용을 마친 오브젝트를 회수하는 함수
	CBaseBullet*		Reuse_MonsterBulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);	// 현재 풀에 있는 오브젝트를 재사용하는 함수
	
	void				Collect_PlayerBulletObj(CBaseBullet* pObj);		// 사용을 마친 오브젝트를 회수하는 함수
	CBaseBullet*		Reuse_PlayerBulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);	// 현재 풀에 있는 오브젝트를 재사용하는 함수

	void				Collect_SphinxBulletObj(CBaseBullet* pObj);
	CBaseBullet*		Reuse_SphinxBulletObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

	void				Collect_GhulObj(CGhul* pObj);
	CGhul*				Reuse_GhulObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

	void				Collect_UIObj(CGameObject* pObj);		// 사용을 마친 오브젝트를 회수하는 함수
	CGameObject*		Reuse_MetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev,_float fPosX, _float fPosY, _float fSpeed, int iTexIndex);	// 현재 풀에 있

private:
	// 오브젝트 풀 쓸때마다 해야되네 두개 같이 쓰면 뻑남
	list<CBaseBullet*>		m_MonsterBulletList;
	list<CBaseBullet*>		m_PlayerBulletList;
	list<CBaseBullet*>		m_SphinxBulletList;
	list<CGhul*>			m_GhulList;
	list<CGameObject*>		m_UIMetroList;



	_int					m_iCount;
	_int					m_iTest = -1;
private:
	virtual void Free(void);
};

