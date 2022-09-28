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
	void				Collect_Obj(CGameObject* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	

	void				Collect_UIObj(CGameObject* pObj);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	// Reuse �� ������ ���¹��
	CGameObject*		Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev,
		 D3DXVECTOR3 vPos);	// ���� Ǯ�� �ִ� ������Ʈ�� �����ϴ� �Լ�

	CGameObject*		Reuse_MetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev,
		_float fPosX, _float fPosY, _float fSpeed, int iTexIndex);	// ���� Ǯ�� ��



private:
	list<CGameObject*>		m_ObjectList;

	list<CGameObject*>		m_UIMetroList;
	//list<CGameObject*>	m_BulletList;  �̷��� � ����°� ���ҵ�
	//list<CGameObject*>	m_MonsterList;  


	_int					m_iCount;
	_int					m_iTest = -1;
private:
	virtual void Free(void);
};

