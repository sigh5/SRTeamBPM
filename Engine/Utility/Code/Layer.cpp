#include "..\..\Header\Layer.h"
#include "MonsterBase.h"

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

void			CLayer::Add_GhulList(CGameObject* pGhul)
{
	m_GhulList.push_back(pGhul);

}

void			CLayer::Add_ObeliskList(CGameObject* pObelisk)
{
	m_ObeliskList.emplace_back(pObelisk);
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
		
		if (iResult == OBJ_DEAD)
		{
			Safe_Release(iter->second);
			iter = m_mapObject.erase(iter);
			
		}
		else
		{
			iter++;
		}
		

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
	for (auto iter = m_GhulList.begin(); iter != m_GhulList.end(); ++iter)
	{
		(*iter)->Update_Object(fTimeDelta);

	}

	for (auto iter = m_EffectList.begin(); iter != m_EffectList.end(); )
	{
		_int iResult = (*iter)->Update_Object(fTimeDelta);

		if (iResult == OBJ_DEAD)
		{
			Safe_Release(*iter);
			iter = m_EffectList.erase(iter);
		}
		else
			iter++;

	}
	ActivevecColliderMonster();




	return iResult;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto& iter : m_mapObject)
		iter.second->LateUpdate_Object();
	for (auto& iter : m_objPoolList)
		iter->LateUpdate_Object();

	for (auto& iter : m_EffectList)
		iter->LateUpdate_Object();

}

void CLayer::initStartCube()
{
	for (auto iter : m_TeleportCubeList[STARTCUBELIST])
	{
		m_RestCubevec.push_back(iter);
	}
}

CGameObject* CLayer::Get_PreRoomTeleCube()
{
	if (m_vecVistedCube.empty())
		return nullptr;

	CGameObject *pTeleEndCube = m_vecVistedCube.top();
	m_vecVistedCube.pop();
	return pTeleEndCube;
}

void CLayer::Clear_Stack()
{
	while (!m_vecVistedCube.empty())
	{
		m_vecVistedCube.pop();
	}
}

void	CLayer::Delete_GhulList()
{
	for (auto iter : m_GhulList)
	{
		Safe_Release(iter);
	}
	m_GhulList.clear();
}

CLayer* CLayer::Create(void)
{
	CLayer*	pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
		Safe_Release(pLayer);

	return pLayer;
}

void		CLayer::Reset_Monster()
{
	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end(); )
	{
		if (nullptr != dynamic_cast<CMonsterBase*>(iter->second))
		{
			if (MONSTER_FLY_HEAD == dynamic_cast<CMonsterBase*>(iter->second)->Get_MonsterType())
			{
				Safe_Release(iter->second);
				iter = m_mapObject.erase(iter);
			}
			else
			{
				dynamic_cast<CMonsterBase*>(iter->second)->Get_BackOriginPos();
				iter++;
			}
		}
		else
			iter++;
	}
}

void		CLayer::Add_vecColliderMonster(CMonsterBase* pMonster)
{
	m_vecColliderMonster.push_back(pMonster);
}

void		CLayer::ActivevecColliderMonster(void)
{
	int vecend = 0;
	vecend = m_vecColliderMonster.size();
	float f_iRadius, f_jRadius, fDistance;
	_vec3 v_iPos, v_jPos;
	for (int i = 0; i < vecend; ++i)
	{
		for (int j = i + 1; j < vecend; ++j)
		{
			f_iRadius = m_vecColliderMonster[i]->Get_Radius();
			f_jRadius = m_vecColliderMonster[j]->Get_Radius();
			v_iPos = m_vecColliderMonster[i]->Get_Pos();
			v_jPos = m_vecColliderMonster[j]->Get_Pos();
			fDistance = sqrtf((powf(v_jPos.x - v_iPos.x, 2) + powf(v_jPos.y - v_iPos.y, 2) + powf(v_jPos.z - v_iPos.z, 2)));
			
				if (f_iRadius + f_jRadius > fDistance)
				{
					_vec3 viToj, vjToi;
					vjToi = v_iPos - v_jPos;
					viToj = v_jPos - v_iPos;
					m_vecColliderMonster[i]->Move_Pos(vjToi * (((f_iRadius + f_jRadius) - fDistance)* 0.2f));
					m_vecColliderMonster[j]->Move_Pos(viToj * (((f_iRadius + f_jRadius) - fDistance)* 0.2f));
				}
			}
	}
	m_vecColliderMonster.clear();
}

void CLayer::ActiveAnubisAttack(const _float & fTimeDelta)
{
	for (auto iter = m_AnubisAttack.begin(); iter != m_AnubisAttack.end();)
	{
		_int iResult = 0;
		iResult = (*iter)->Update_Object(fTimeDelta);
		if (iResult == 1)
		{
			Safe_Release((*iter));
			iter = m_AnubisAttack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::ActiveSpike(const _float & fTimeDelta)
{
	for (auto iter = m_EarthSpike.begin(); iter != m_EarthSpike.end();)
	{
		_int iResult = 0;
		iResult = (*iter)->Update_Object(fTimeDelta);
		if (iResult == 1)
		{
			Safe_Release((*iter));
			iter = m_EarthSpike.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::Add_AnubisAttack(CGameObject * pThunder)
{
	m_AnubisAttack.push_back(pThunder);
}
void		CLayer::Add_Spike(CGameObject* pSpike)
{
	m_EarthSpike.push_back(pSpike);
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

	for (auto iter : m_GhulList)
	{
		Safe_Release(iter);
	}
	m_GhulList.clear();
	m_ObeliskList.clear();

	

	for (auto& iter : m_EffectList)
		Safe_Release(iter);
	m_EffectList.clear();
	m_vecColliderMonster.clear();
	for (auto iter : m_AnubisAttack)
	{
		Safe_Release(iter);
	}
	m_AnubisAttack.clear();
		for (auto iter : m_EarthSpike)
		{
			Safe_Release(iter);
		}
	m_EarthSpike.clear();
}
