#include "stdafx.h"
#include "ObjectMgr.h"

#include "Export_Function.h"


#include "MetronomeUI.h"
#include "MonsterBullet.h"
#include "Bullet.h"

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
	
}


CObjectMgr::~CObjectMgr()
{
	Free();
}

void CObjectMgr::Collect_BulletObj(CBaseBullet * pObj)
{
	if (nullptr == pObj)
		return;

	m_BulletList.push_back(pObj);
}


CBaseBullet* CObjectMgr::Reuse_BulltObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos,BULLET_ID eID)
{
	CBaseBullet*		pObject = nullptr;
	if (m_BulletList.empty())
	{
		if(eID == MONSTER_BULLET)
			pObject = CMonsterBullet::Create(pGraphicDev, vPos);
		else if (eID == PLAYER_BULLET)
			pObject = CBullet::Create(pGraphicDev, vPos);
	}
	else
	{
		pObject = m_BulletList.front();	
		if (eID == MONSTER_BULLET)
			dynamic_cast<CMonsterBullet*>(pObject)->Set_MoveDir(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC, &vPos,MONSTER_BULLET);
		else if(eID ==PLAYER_BULLET)
			dynamic_cast<CBullet*>(pObject)->Set_MoveDir(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC, &vPos,PLAYER_BULLET);

		m_BulletList.pop_front();
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
		++m_iCount;
	}
	else
	{
		pObject = m_UIMetroList.front();
		dynamic_cast<CMetronomeUI*>(pObject)->init(fPosX, fPosY, fSpeed, iTexIndex);
		m_UIMetroList.pop_front();
	}


	return pObject;
}

void CObjectMgr::Free(void)
{
	for_each(m_BulletList.begin(), m_BulletList.end(), CDeleteObj());
	m_BulletList.clear();

	for_each(m_UIMetroList.begin(), m_UIMetroList.end(), CDeleteObj());
	m_UIMetroList.clear();

}
