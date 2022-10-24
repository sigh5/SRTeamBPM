#include "stdafx.h"
#include "..\Header\MagneticField.h"

#include "Export_Function.h"
#include "Terrain.h"

#include "TeleCube.h"
#include "ControlRoom.h"
#include "SkyBox.h"
#include "FireTrap.h"
#include "FirePillar.h"
#include "ShopUI.h"
#include "StageTerrain.h"
#include "MiniPlayer.h"
#include "MonsterMini.h"
#include "MiniMonster2.h"
#include "MiniMonster3.h"

CMagneticField::CMagneticField(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMagneticField::~CMagneticField()
{
}

HRESULT CMagneticField::Ready_Object(const _vec3 & vCenter)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;

	m_pTransCom->Set_Pos(vCenter.x, vCenter.y + 4.f, vCenter.z);
	vScale = { 10.f,10.f,10.f };

	m_pTransCom->Set_Scale(&vScale);

	m_pColliderCom->Set_HitRadiuos(5.f);
	m_pColliderCom->Set_vCenter(const_cast<_vec3*>(&vCenter), &vScale);
	return S_OK;
}

_int CMagneticField::Update_Object(const _float & fTimeDelta)
{
	m_pColliderCom->Set_HitBoxMatrix(&(m_pTransCom->m_matWorld));

	if (m_bSetActive)
	{
		m_fFrame += 1.f *fTimeDelta;

		if (m_fFrame >= 5.f)
		{
			m_fFrame = 0.f;
			m_bPlayerInTerrain = false;
	
			m_bSetActive = false;
		}
	}


	if (m_bPlayerInTerrain)
	{
		m_fFrame += 1.f *fTimeDelta;

		if (m_fFrame >= 5.f)
		{
			m_fFrame = 0.f;
			m_bPlayerInTerrain = false;

			m_bSetActive = false;
		}

	}
	else
	{
		if (m_iMonsterMaxNum < 100)
		{
			m_fMonsterSpawnTimer += 1.f *fTimeDelta;
			
			_int iMonsterIndex = rand() % 3;


			if (iMonsterIndex == 0)
			{
				if (m_fMonsterSpawnTimer >= 5.f)
				{
					_vec3 vPos;
					m_pTransCom->Get_Info(INFO_POS, &vPos);
					CScene* pScene = Get_Scene();
					CLayer*		pLayer = pScene->GetLayer(L"Layer_Monster");
					CGameObject*		pGameObject = nullptr;

					pGameObject = CMonsterMini::Create(m_pGraphicDev);
					pLayer->Add_EffectList(pGameObject);
					static_cast<CMonsterMini*>(pGameObject)->Set_Pos(vPos);

					m_fMonsterSpawnTimer = 0.f;
					++m_iMonsterMaxNum;
				}
			}
			else if (iMonsterIndex  == 1)
			{
				if (m_fMonsterSpawnTimer >= 5.f)
				{
					_vec3 vPos;
					m_pTransCom->Get_Info(INFO_POS, &vPos);
					CScene* pScene = Get_Scene();
					CLayer*		pLayer = pScene->GetLayer(L"Layer_Monster");
					CGameObject*		pGameObject = nullptr;

					pGameObject = CMiniMonster3::Create(m_pGraphicDev);
					pLayer->Add_EffectList(pGameObject);
					static_cast<CMiniMonster3*>(pGameObject)->Set_Pos(vPos);

					m_fMonsterSpawnTimer = 0.f;
					++m_iMonsterMaxNum;
				}
			}

			else if (iMonsterIndex  == 2)
			{
				if (m_fMonsterSpawnTimer >= 5.f)
				{
					_vec3 vPos;
					m_pTransCom->Get_Info(INFO_POS, &vPos);
					CScene* pScene = Get_Scene();
					CLayer*		pLayer = pScene->GetLayer(L"Layer_Monster");
					CGameObject*		pGameObject = nullptr;

					pGameObject = CMiniMonster2::Create(m_pGraphicDev);
					pLayer->Add_EffectList(pGameObject);
					static_cast<CMiniMonster2*>(pGameObject)->Set_Pos(vPos);

					m_fMonsterSpawnTimer = 0.f;
					++m_iMonsterMaxNum;
				}
			}

		}
		
	}



	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;

}


void CMagneticField::LateUpdate_Object()
{
}

void CMagneticField::Render_Obejct(void)
{
	if (!m_bSetActive)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
	//m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

void CMagneticField::Collision_Event()
{


	if (!m_bSetActive)
		return;

	CScene* pScene = Get_Scene();
	CLayer* pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject* pPlayer = pLayer->Get_GameObject(L"Player");
	CCollider* pCollider = dynamic_cast<CCollider*>(pPlayer->Get_Component(L"Proto_ColliderCom", ID_STATIC));


	if (m_pColliderCom->Check_CollisonUseCollider(pCollider, m_pColliderCom))
	{
		m_bPlayerInTerrain = true;
	}
	else
		m_bPlayerInTerrain = false;


	if (m_bPlayerInTerrain)
	{
		CScene* pScene = Get_Scene();
		CLayer* pLayer = pScene->GetLayer(L"Layer_Monster");
		CMiniPlayer* pPlayer = dynamic_cast<CMiniPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
		for (auto iter = pLayer->Get_EffectList().begin(); iter != pLayer->Get_EffectList().end(); ++iter)
		{
			CCollider* pCollider = dynamic_cast<CCollider*>((*iter)->Get_Component(L"Proto_ColliderCom", ID_STATIC));
			if (m_pColliderCom->Check_CollisonUseCollider(pCollider, m_pColliderCom))
			{
				(*iter)->Set_ObjDead(true);
				pPlayer->Set_MonsterKillCount();
			}
		}

	}

	


}

HRESULT CMagneticField::Add_Component(void)
{
	ADD_CLONE_PROTO(CSphereTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_SphereTexCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");
	ADD_CLONE_PROTO(CCalculator, m_pCalculatorCom, m_mapComponent, ID_STATIC, L"Proto_CalculatorCom");
	ADD_CLONE_PROTO(CCollider, m_pColliderCom, m_mapComponent, ID_STATIC, L"Proto_ColliderCom");

	return S_OK;
}

CMagneticField * CMagneticField::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vCenter)
{
	CMagneticField *	pInstance = new CMagneticField(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vCenter)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CMagneticField::Free()
{
	CGameObject::Free();
}
