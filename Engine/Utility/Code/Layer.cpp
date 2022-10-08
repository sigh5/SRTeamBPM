#include "..\..\Header\Layer.h"

USING(Engine)

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	Free();
}

CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

HRESULT CLayer::Delete_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return E_FAIL;

	CGameObject * pInstance = iter->second;

	Safe_Release<CGameObject*>(pInstance);

	iter = m_mapObject.erase(iter);

	return S_OK;

}

HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;

	m_mapObject.insert({ pObjTag, pInstance });

	return S_OK;
}

HRESULT CLayer::Add_GameObjectList(CGameObject * pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;


	m_objPoolList.push_back(pInstance);


	return S_OK;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int iResult = 0;

	
	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end(); )
	{
		iResult = iter->second->Update_Object(fTimeDelta);

		if (iResult & 0x80000000)
				return iResult;
		
		iter++;

	}
	
	for (auto &iter = m_objPoolList.begin(); iter != m_objPoolList.end();)
	{
		int iResult = (*iter)->Update_Object(fTimeDelta);
	
		if (iResult & 0x80000000)
			return iResult;
		
		else if (iResult == 5)
		{
			m_objPoolList.erase(iter++);
			continue;
		}
		++iter;

	
	}



	return iResult;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto& iter : m_mapObject)
		iter.second->LateUpdate_Object();
	for (auto& iter : m_objPoolList)
		iter->LateUpdate_Object();
}

CLayer* CLayer::Create(void)
{
	CLayer*	pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
		Safe_Release(pLayer);

	return pLayer;
}

void CLayer::Free(void)
{

	for (auto iter : NameList)
	{
		Safe_Delete_Array(iter);
	}
	NameList.clear();
	
	for (auto iter : m_objPoolList)
	{
		Safe_Release(iter);
	}
	m_objPoolList.clear();

	for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
	{
		for_each(m_TeleportCubeList[i].begin(), m_TeleportCubeList[i].end(), CDeleteObj());
		m_TeleportCubeList[i].clear();
	}


	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();


	

}
