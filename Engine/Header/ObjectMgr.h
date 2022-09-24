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
	void				Collect_Obj(CGameObject* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	CGameObject*		Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXVECTOR3& vPos);	// ���� Ǯ�� �ִ� ������Ʈ�� �����ϴ� �Լ�

private:
	list<CGameObject*>		m_ObjectList;
	//list<CGameObject*>	m_BulletList;  �̷��� � ����°� ���ҵ�
	//list<CGameObject*>	m_MonsterList;  

private:
	virtual void Free(void);
};

END