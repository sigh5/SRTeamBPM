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
	void				Collect_MonsterBulletObj(CBaseBullet* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	CBaseBullet*		Reuse_MonsterBulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);	// ���� Ǯ�� �ִ� ������Ʈ�� �����ϴ� �Լ�
	
	void				Collect_PlayerBulletObj(CBaseBullet* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	CBaseBullet*		Reuse_PlayerBulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);	// ���� Ǯ�� �ִ� ������Ʈ�� �����ϴ� �Լ�

	void				Collect_SphinxBulletObj(CBaseBullet* pObj);
	CBaseBullet*		Reuse_SphinxBulletObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

	void				Collect_GhulObj(CGhul* pObj);
	CGhul*				Reuse_GhulObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

	void				Collect_UIObj(CGameObject* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	CGameObject*		Reuse_MetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev,_float fPosX, _float fPosY, _float fSpeed, int iTexIndex);	// ���� Ǯ�� ��

private:
	// ������Ʈ Ǯ �������� �ؾߵǳ� �ΰ� ���� ���� ����
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

