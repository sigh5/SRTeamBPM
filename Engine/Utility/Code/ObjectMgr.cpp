#include "..\..\Header\ObjectMgr.h"

#include "Export_Function.h"

USING(Engine)
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

CGameObject * CObjectMgr::Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXVECTOR3 & vPos)
{
	CGameObject*		pObject = nullptr;

	if (m_ObjectList.empty())
	{
		pObject = CGameObject::Create(pGraphicDev, vPos);
		//pObject = new CGameObject(pGraphicDev, vPos);
	}

	else
	{
		pObject = m_ObjectList.front();
		m_ObjectList.pop_front();
	}

	dynamic_cast<CGameObject*>(pObject)->Set_Pos(vPos);
	//dynamic_cast<CGameObject*>(pObject)->Set_Dir(vDir);

	return pObject;
}

void CObjectMgr::Free(void)
{
	for_each(m_ObjectList.begin(), m_ObjectList.end(), CDeleteObj());
	m_ObjectList.clear();
}
