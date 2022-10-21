#include "stdafx.h"
#include "..\Header\ControlRoom.h"

#include "Export_Function.h"
#include "Terrain.h"

#include "TeleCube.h"
#include "ControlRoom.h"
#include "SkyBox.h"
#include "FireTrap.h"
#include "FirePillar.h"

CControlRoom::CControlRoom(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CControlRoom::~CControlRoom()
{
}

HRESULT CControlRoom::Ready_Object(const _vec3& vCenter)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;

	m_pTransCom->Set_Pos(vCenter.x, vCenter.y +4.f,vCenter.z);
	vScale = { 85.f,85.f,85.f };

	m_pTransCom->Set_Scale(&vScale);
	
	
	m_pColliderCom->Set_HitRadiuos(45.f);
	m_pColliderCom->Set_vCenter(const_cast<_vec3*>(&vCenter), &vScale);
	return S_OK;
}

_int CControlRoom::Update_Object(const _float & fTimeDelta)
{
	m_pColliderCom->Set_HitBoxMatrix(&(m_pTransCom->m_matWorld));


	CScene *pScene = Get_Scene();
	CLayer* pLayer = pScene->GetLayer(L"Layer_Environment");
	CSkyBox* pSkyBox = static_cast<CSkyBox*>(pLayer->Get_GameObject(L"SkyBox"));

	if (pSkyBox->Get_controlCubeCheck())
	{
		m_fCollisionTimer += 1.f*fTimeDelta;
	}

	if (m_fCollisionTimer >= 7.f)
	{
		pSkyBox->Set_ControlCubeCheck(false);
		m_fCollisionTimer = 0;
	}

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_PRIORITY, this);
	
	return 0;
}

void CControlRoom::LateUpdate_Object()
{

	CScene *pScene = Get_Scene();
	CLayer* pLayer = pScene->GetLayer(L"Layer_Environment");
	CSkyBox* pSkyBox = static_cast<CSkyBox*>(pLayer->Get_GameObject(L"SkyBox"));


	if (m_bPlayerInTerrain)
	{
		if (m_iRestMonsterNum == 0 && !pSkyBox->Get_controlCubeCheck())
		{
			CScene* pScene = Get_Scene();
			CLayer* pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
			for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
			{
				for (auto iter : *(pLayer->Get_TeleCubeList(i)))
					dynamic_cast<CTeleCube*>(iter)->Set_Active(false);
			}
			m_bPlayerInTerrain = false;
			pSkyBox->Set_ControlCubeCheck(true);
		}
		
	}
	m_iRestMonsterNum = 0;
	m_pCurrentRoomMonster.clear();
	Set_Light_Obj();
}

void CControlRoom::Render_Obejct(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
	////m_pColliderCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

void CControlRoom::Collision_Event()
{
	// 거리안에있는 몬스터들 다체킹하는 로직

	CScene* pScene = Get_Scene();
	CLayer* pLayer = pScene->GetLayer(L"Layer_GameLogic");

	CGameObject* pPlayer = pLayer->Get_GameObject(L"Player");

	CCollider* pCollider = dynamic_cast<CCollider*>(pPlayer->Get_Component(L"Proto_ColliderCom", ID_STATIC));

	if (m_pColliderCom->Check_CollisonUseCollider(pCollider, m_pColliderCom))
		m_bPlayerInTerrain = true;
	else
		m_bPlayerInTerrain = false;
	
	if (m_bPlayerInTerrain)
	{
		for (auto iter : pLayer->Get_GhulList())
		{
			CCollider* pCollider = dynamic_cast<CCollider*>(iter->Get_Component(L"Proto_ColliderCom", ID_STATIC));
			
			if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
			{
				++m_iRestMonsterNum;
				
				if (static_cast<CMonsterBase*>(iter)->Get_InfoRef()._iHp <= 0)
					--m_iRestMonsterNum;
				else
				{
					m_pCurrentRoomMonster.push_back(iter);
				}
			}
		}
		
		CGameObject* pGameobj = nullptr;
		for (auto iter : pLayer->Get_GameObjectMap())	// 함정일때 뺴야됌
		{
			CMonsterBase* pMonster = dynamic_cast<CMonsterBase*>(iter.second);

			if (pMonster == nullptr)
				continue;
			
			CCollider* pCollider = dynamic_cast<CCollider*>((pMonster)->Get_Component(L"Proto_ColliderCom", ID_STATIC));

			if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
			{
				CFireTrap* pTrap = dynamic_cast<CFireTrap*>(iter.second);
				if (pTrap != nullptr)
					continue;

				++m_iRestMonsterNum;

				if (static_cast<CMonsterBase*>(iter.second)->Get_InfoRef()._iHp <= 0)
					--m_iRestMonsterNum;
				else
				{
					m_pCurrentRoomMonster.push_back(iter.second);
				}
			}

		}
		
		for (auto iter : pLayer->Get_ObeliskList())
		{
			CCollider* pCollider = dynamic_cast<CCollider*>((iter)->Get_Component(L"Proto_ColliderCom", ID_STATIC));

			if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
			{
				++m_iRestMonsterNum;
			
				if (static_cast<CMonsterBase*>(iter)->Get_InfoRef()._iHp <= 0)
				{
					--m_iRestMonsterNum;
				}
				
			}
		}

		
	}


}

void CControlRoom::Area_of_Effect()
{
	if (m_bPlayerInTerrain)
	{
		for (auto iter : m_pCurrentRoomMonster)
		{
			dynamic_cast<CMonsterBase*>(iter)->Excution_Event(true);
		}
	}


}

HRESULT CControlRoom::Add_Component(void)
{
	ADD_CLONE_PROTO(CSphereTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_SphereTexCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");
	ADD_CLONE_PROTO(CCalculator, m_pCalculatorCom, m_mapComponent, ID_STATIC, L"Proto_CalculatorCom");
	ADD_CLONE_PROTO(CCollider, m_pColliderCom, m_mapComponent, ID_STATIC, L"Proto_ColliderCom");
	


	return S_OK;
}

HRESULT CControlRoom::SetUp_Material(void)
{
	return E_NOTIMPL;
}

_bool CControlRoom::Set_Light_Obj()
{
	if (m_bPlayerInTerrain)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		
		D3DLIGHT9		tLightInfo;
		ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

		tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
		tLightInfo.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		tLightInfo.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		tLightInfo.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		tLightInfo.Direction = _vec3(1.f, 0.f, -1.f);
		tLightInfo.Position = vPos;
		
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), );
		
		
		D3DLIGHT9		tLightInfo1;
		ZeroMemory(&tLightInfo1, sizeof(D3DLIGHT9));

		tLightInfo1.Type = D3DLIGHT_DIRECTIONAL;
		tLightInfo1.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		tLightInfo1.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		tLightInfo1.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		tLightInfo1.Direction = _vec3(-1.f, 0.f, 1.f);
		tLightInfo1.Position = vPos;
	
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo1, 1), );
		

		//D3DLIGHT9		tLightInfo2;
		//ZeroMemory(&tLightInfo1, sizeof(D3DLIGHT9));

		//tLightInfo2.Type = D3DLIGHT_DIRECTIONAL;
		//tLightInfo2.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		//tLightInfo2.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		//tLightInfo2.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		///*tLightInfo2.Direction = _vec3(0.f, -1.f, 0.f);
		//tLightInfo2.Position = vPos;*/

		//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo2, 2), );


		return true;
	}


	return false;

}

CControlRoom * CControlRoom::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vCenter)
{
	CControlRoom *	pInstance = new CControlRoom(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vCenter)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CControlRoom::Free()
{
	CGameObject::Free();
}
