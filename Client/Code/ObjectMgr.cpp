#include "stdafx.h"
#include "ObjectMgr.h"

#include "Export_Function.h"


#include "MetronomeUI.h"
//#include ""

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
	
}


CObjectMgr::~CObjectMgr()
{
	Free();
}

void CObjectMgr::Collect_Obj(CGameObject * pObj)
{
	if (nullptr == pObj)
		return;

	m_BulletList.push_back(pObj);
}

CGameObject * CObjectMgr::Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3  vPos)
{


	return nullptr;
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
