#include "stdafx.h"
#include "ObjectMgr.h"

#include "Export_Function.h"


#include "MetronomeUI.h"
#include "MonsterBullet.h"
#include "Bullet.h"
#include "SphinxBullet.h"
#include "Ghul.h"
#include "MetronomeSmallUI.h"
#include "MetronomeRect.h"
#include "Player_Dead_UI.h"


IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
	
}


CObjectMgr::~CObjectMgr()
{
	Free();
}

void CObjectMgr::Collect_MonsterBulletObj(CBaseBullet * pObj)
{
	if (nullptr == pObj)
		return;

	m_MonsterBulletList.push_back(pObj);
}

void CObjectMgr::Collect_PlayerBulletObj(CBaseBullet * pObj)
{
	if (nullptr == pObj)
		return;

	m_PlayerBulletList.push_back(pObj);

}


void CObjectMgr::Collect_SphinxBulletObj(CBaseBullet * pObj)
{
	if (nullptr == pObj)
		return;

	m_SphinxBulletList.push_back(pObj);
}

CBaseBullet * CObjectMgr::Reuse_SphinxBulletObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBaseBullet*		pObject = nullptr;
	if (m_SphinxBulletList.empty())
	{
		pObject = CSphinxBullet::Create(pGraphicDev, vPos);
	}
	else
	{
		pObject = m_SphinxBulletList.front();
		_vec3 vScale = { 0.5f,0.5f,0.5f };
		dynamic_cast<CSphinxBullet*>(pObject)->Set_MoveDir(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC, &vPos, MONSTER_BULLET, &vScale);
		m_SphinxBulletList.pop_front();
	}
	return pObject;
}

void CObjectMgr::Collect_GhulObj(CGhul * pObj)
{
	if (nullptr == pObj)
		return;

	m_GhulList.push_back(pObj);
}

CGhul * CObjectMgr::Reuse_GhulObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CGhul*		pObject = nullptr;
	if (m_GhulList.empty())
	{
		pObject = CGhul::Create(pGraphicDev, (_float)vPos.x, (_float)vPos.z);
	}
	else
	{
		pObject = m_GhulList.front();

		CTransform* pTransfrom;
		pTransfrom = static_cast<CTransform*>(pObject->Get_Component(L"Proto_DynamicTransformCom",ID_DYNAMIC));
		pTransfrom->Set_Pos(vPos.x, vPos.y, vPos.z);
		m_GhulList.pop_front();
	}
	return pObject;
}

CBaseBullet * CObjectMgr::Reuse_MonsterBulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBaseBullet*		pObject = nullptr;
	if (m_MonsterBulletList.empty())
	{
		pObject = CMonsterBullet::Create(pGraphicDev, vPos);
	}
	else
	{
		pObject = m_MonsterBulletList.front();
		_vec3 vScale = { 0.5f,0.5f,0.5f };
		dynamic_cast<CMonsterBullet*>(pObject)->Set_MoveDir(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC, &vPos, MONSTER_BULLET, &vScale);
		m_MonsterBulletList.pop_front();
	}
	return pObject;
}

CBaseBullet * CObjectMgr::Reuse_PlayerBulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBaseBullet*		pObject = nullptr;
	if (m_PlayerBulletList.empty())
	{
		pObject = CBullet::Create(pGraphicDev, vPos);
	}
	else
	{
		pObject = m_PlayerBulletList.front();
		_vec3 vScale = { 0.5f,0.5f,0.5f };
		dynamic_cast<CBullet*>(pObject)->Set_MoveDir(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC, &vPos, PLAYER_BULLET, &vScale);
		m_PlayerBulletList.pop_front();
	}
	return pObject;
}


void CObjectMgr::Collect_UIObj(CGameObject * pObj)
{
	if (nullptr == pObj)
		return;

	m_UIMetroList.push_back(pObj);

}

CGameObject * CObjectMgr::Reuse_MetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev, _float fPosX, _float fPosY, _float fSpeed, int iTexIndex)
{
	CGameObject*		pObject = nullptr;

	if(m_UIMetroList.empty())
	{
		pObject = CMetronomeUI::Create(pGraphicDev, fPosX, fPosY, fSpeed, iTexIndex);
	}
	else
	{
		pObject = m_UIMetroList.front();
		dynamic_cast<CMetronomeUI*>(pObject)->init(fPosX, fPosY, fSpeed, iTexIndex,150);
		m_UIMetroList.pop_front();
	}


	return pObject;
}

void CObjectMgr::Collect_UISmallObj(CGameObject * pObj)
{
	if (nullptr == pObj)
		return;

	m_UISmallMetroList.push_back(pObj);
}

CGameObject * CObjectMgr::Reuse_MetronomeSmallUI(LPDIRECT3DDEVICE9 pGraphicDev, _float fPosX, _float fPosY, _float fSpeed, int iTexIndex)
{
	CGameObject*		pObject = nullptr;
	if (m_UISmallMetroList.empty())
	{
		pObject = CMetronomeSmallUI::Create(pGraphicDev, fPosX, fPosY, fSpeed, iTexIndex);
	}
	else
	{
		pObject = m_UISmallMetroList.front();
		dynamic_cast<CMetronomeSmallUI*>(pObject)->init(fPosX, fPosY, fSpeed, iTexIndex, 150);
	
		m_UISmallMetroList.pop_front();
	}

	return pObject;
}

void CObjectMgr::Collect_UIRect(CGameObject * pObj)
{
	if (nullptr == pObj)
		return;

	m_UIMetroRectList.push_back(pObj);
}


CGameObject * CObjectMgr::Reuse_MetronomeRectUI(LPDIRECT3DDEVICE9 pGraphicDev, int iTexIndex)
{
	CGameObject*		pObject = nullptr;
	if (m_UIMetroRectList.empty())
	{
		pObject =CMetronomeRect::Create(pGraphicDev, iTexIndex);
		++m_iCount;
	}
	else
	{
		pObject = m_UIMetroRectList.front();
		dynamic_cast<CMetronomeRect*>(pObject)->init(iTexIndex, 150);
		m_UIMetroRectList.pop_front();
	}

	return pObject;
}



void CObjectMgr::Free(void)
{
	
	for_each(m_PlayerBulletList.begin(), m_PlayerBulletList.end(), CDeleteObj());
	m_PlayerBulletList.clear();

	for_each(m_MonsterBulletList.begin(), m_MonsterBulletList.end(), CDeleteObj());
	m_MonsterBulletList.clear();

	for_each(m_SphinxBulletList.begin(), m_SphinxBulletList.end(), CDeleteObj());
	m_SphinxBulletList.clear();


	for_each(m_UIMetroList.begin(), m_UIMetroList.end(), CDeleteObj());
	m_UIMetroList.clear();

	for_each(m_UISmallMetroList.begin(), m_UISmallMetroList.end(), CDeleteObj());
	m_UISmallMetroList.clear();
	
	for_each(m_UIMetroRectList.begin(), m_UIMetroRectList.end(), CDeleteObj());
	m_UIMetroRectList.clear();

	

}
