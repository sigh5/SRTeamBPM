#include "stdafx.h"
#include "..\Header\ControlRoom.h"

#include "Export_Function.h"
#include "Terrain.h"

#include "TeleCube.h"
#include "ControlRoom.h"
#include "SkyBox.h"


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
	vScale = { 90.f,90.f,90.f };

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

	if (m_fCollisionTimer > 5.f)
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
		cout << m_iRestMonsterNum << endl;
		cout << Temt << endl;
		m_iRestMonsterNum = 0;
		Temt = 0;
	}
	
}

void CControlRoom::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
	//m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

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
			}
		}
		
		for (auto iter : pLayer->Get_GameObjectMap())
		{
			CMonsterBase* pMonster = dynamic_cast<CMonsterBase*>(iter.second);

			if (pMonster == nullptr)
				continue;
			
			CCollider* pCollider = dynamic_cast<CCollider*>((pMonster)->Get_Component(L"Proto_ColliderCom", ID_STATIC));

			if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
			{
				++m_iRestMonsterNum;

				if (static_cast<CMonsterBase*>(iter.second)->Get_InfoRef()._iHp <= 0)
					--m_iRestMonsterNum;
			}

		}
		
		for (auto iter : pLayer->Get_ObeliskList())
		{
			CCollider* pCollider = dynamic_cast<CCollider*>((iter)->Get_Component(L"Proto_ColliderCom", ID_STATIC));

			if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
			{
				++m_iRestMonsterNum;
				++Temt;
				if (static_cast<CMonsterBase*>(iter)->Get_InfoRef()._iHp <= 0)
				{
					--m_iRestMonsterNum;
					--Temt;
				}
			}
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
