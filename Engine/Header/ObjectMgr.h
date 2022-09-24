#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CObjectMgr :public CBase
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	explicit CObjectMgr();
	virtual ~CObjectMgr();

public:
	void				Collect_Obj(CGameObject* pObj);		// 사용을 마친 오브젝트를 회수하는 함수
	CGameObject*		Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXVECTOR3& vPos);	// 현재 풀에 있는 오브젝트를 재사용하는 함수

private:
	list<CGameObject*>		m_ObjectList;
	//list<CGameObject*>	m_BulletList;  이렇게 몇개 만드는게 편할듯
	//list<CGameObject*>	m_MonsterList;  

private:
	virtual void Free(void);
};

END