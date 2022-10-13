#include "stdafx.h"
#include "..\Header\Box.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Coin.h"
#include "HealthPotion.h"
#include "Player.h"
#include "MyCamera.h"

USING(Engine)

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_bBoxOpen(true)
{
}

CBox::~CBox()
{
}

HRESULT CBox::Ready_Object(_uint iX, _uint iY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iY);
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(1, 0, 0.2f); // Animation 적용을 위한 기본값

	return S_OK;
}

_int CBox::Update_Object(const _float & fTimeDelta)
{
	_vec3 vecScale = { 1.5f, 1.5f, 1.0f };

	m_pTransCom->Set_Scale(&vecScale);

	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	//Set_OnTerrain();
	
	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CBox::LateUpdate_Object(void)
{
	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);

	// 
	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransCom->Set_WorldMatrix(&(matWorld));

}

void CBox::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBox::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );
	
	if (m_pColliderCom->Check_Collision(this, pGameObject,1,1))
	{
		if (Engine::Key_Down(DIK_F))
		{
			CAnimation* pBoxAnimation = dynamic_cast<CAnimation*>(pGameObject->Get_Component(L"Proto_AnimationCom", ID_STATIC));

			Open_Event(pGameObject);
		}
	}

}



void CBox::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight+0.9f, vPos.z);

}

HRESULT CBox::Open_Event(CGameObject * pGameObject)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	_vec3 vObjPos;
	_vec3 vPlayerPos;

	pTransform->Get_Info(INFO_POS, &vObjPos);
	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
	// ★
	if (m_bBoxOpen == true && m_pColliderCom->Check_Sphere_InterSect(vObjPos, vPlayerPos, 1.f, 1.f) == true )
	{
		m_pAnimationCom->Open_Box_Animation(m_bBoxOpen);
		m_bBoxOpen = false;
		_vec3 vPos;
		
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		CGameObject* pGameObj = nullptr;
		/*
		pGameObj = READY_LAYER_POS(pGameObj, CHealthPotion, pMyLayer, m_pGraphicDev, L"HealthPotion", (_uint)vPos.x + 2, (_uint)vPos.z + 2);

		pGameObj = READY_LAYER_POS(pGameObj, CCoin, pMyLayer, m_pGraphicDev, L"Coin", (_uint)vPos.x+2 , (_uint)vPos.z + 4);
*/
		if (m_bBoxOpen && rand() % 3 == 0)
		{
			// HealthPotion
			/*pGameObj = CHealthPotion::Create(m_pGraphicDev, (_uint)vPos.x + 1, (_uint)vPos.z + 3);
			NULL_CHECK_RETURN(pGameObj, E_FAIL);
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(L"HealthPotion", pGameObj), E_FAIL);*/
		pGameObj = READY_LAYER_POS(pGameObj, CHealthPotion, pMyLayer, m_pGraphicDev, L"HealthPotion", 100, 100);

			
		
		}
		else if (m_bBoxOpen && rand() % 3 == 1)
		{
			// Coin
			pGameObj = CCoin::Create(m_pGraphicDev, (_uint)vPos.x + 1, (_uint)vPos.z + 3);
			NULL_CHECK_RETURN(pGameObj, E_FAIL);
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(L"Coin", pGameObj), E_FAIL);
		}
		
		else if (m_bBoxOpen && rand() % 3 == 2)
		{
			return S_OK;
		}

	
	}
	return S_OK;
}

HRESULT CBox::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BoxTexture", m_mapComponent, ID_STATIC);

	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);


	return S_OK;
}



CBox * CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY)
{
	CBox*	pInstance = new CBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CBox::Free(void)
{
	CGameObject::Free();
}
