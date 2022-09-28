#include "stdafx.h"
#include "ObjectMgr.h"

#include "Export_Function.h"


#include "MetronomeUI.h"







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

	m_ObjectList.push_back(pObj);
}

CGameObject * CObjectMgr::Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev,  D3DXVECTOR3  vPos)
{
	CGameObject*		pObject = nullptr;

	//if (m_ObjectList.empty())
	//{
	//
	//	

	//}

	//else
	//{
	//	pObject = m_ObjectList.front();
	//	m_ObjectList.pop_front();
	//}



	return pObject;
}

CGameObject * CObjectMgr::Reuse_MetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev, _float fPosX, _float fPosY, _float fSpeed, int iTexIndex)
{
	CGameObject*		pObject = nullptr;
	if(m_ObjectList.empty())
	{
		pObject = CMetronomeUI::Create(pGraphicDev, fPosX, fPosY, fSpeed, iTexIndex);
		++m_iCount;
	}
	else
	{
		pObject = m_ObjectList.front();
		dynamic_cast<CMetronomeUI*>(pObject)->init(fPosX, fPosY, fSpeed, iTexIndex);
		m_ObjectList.pop_front();
	}


	return pObject;
}

void CObjectMgr::Free(void)
{
	for_each(m_ObjectList.begin(), m_ObjectList.end(), CDeleteObj());
	m_ObjectList.clear();
}
