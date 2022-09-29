#pragma once
#include "Base.h"
#include "GameObject.h"

#include "BaseBullet.h"

class CObjectMgr :public CBase
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	explicit CObjectMgr();
	virtual ~CObjectMgr();

public:
	

public:
	void				Collect_BulletObj(CBaseBullet* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	CBaseBullet*		Reuse_BulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos,BULLET_ID eID);	// ���� Ǯ�� �ִ� ������Ʈ�� �����ϴ� �Լ�

	void				Collect_UIObj(CGameObject* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	CGameObject*		Reuse_MetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev,_float fPosX, _float fPosY, _float fSpeed, int iTexIndex);	// ���� Ǯ�� ��

private:
	list<CBaseBullet*>		m_BulletList;
	list<CGameObject*>		m_UIMetroList;



	_int					m_iCount;
	_int					m_iTest = -1;
private:
	virtual void Free(void);
};

