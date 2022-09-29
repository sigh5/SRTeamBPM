#include "stdafx.h"
#include "..\Header\HWPlayer.h"
#include "Bullet.h"
#include "Bullet_UI.h"

#include "Export_Function.h"
#include "ObjectMgr.h"

CHWPlayer::CHWPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
{
}

CHWPlayer::~CHWPlayer()
{
}

HRESULT CHWPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_vec3 vScale = { 0.5f, 0.5f, 0.5f };
	m_pTransCom->Set_Pos(10.f, 0.f, 10.f);
	m_pTransCom->Set_Scale(&vScale);
	m_pTransCom->Update_Component(1.f);
	// 움직이기 위한 변수
	return S_OK;
}

_int CHWPlayer::Update_Object(const _float & fTimeDelta)
{

	Key_Input(fTimeDelta);
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_fFrame += 1.f * fTimeDelta;

	if (m_fFrame >= 1.f)
		m_fFrame = 0.f;



	Add_RenderGroup(RENDER_ALPHA, this);
	
	return 0;
}

void CHWPlayer::LateUpdate_Object(void)
{

	Set_OnTerrain();
}

void CHWPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);	// Stage 용

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
}

HRESULT CHWPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });
	
	// Stage 용
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });


	return S_OK;
}

void CHWPlayer::Key_Input(const _float & fTimeDelta)
{
	m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);
	m_pTransCom->Get_Info(INFO_UP, &m_vUp);
	m_pTransCom->Get_Info(INFO_POS, &m_vPos);

	if (Get_DIKeyState(DIK_W) & 0X80)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * 10.f * fTimeDelta));
		m_eDirType = DIR_UP;
	}

	if (Get_DIKeyState(DIK_S) & 0X80)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * -10.f * fTimeDelta));
		m_eDirType = DIR_DOWN;
	}

	if (Get_DIKeyState(DIK_A) & 0X80)
	{
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		
		m_pTransCom->Move_Pos(&(vRight * 10.f * fTimeDelta));
		m_eDirType = DIR_LEFT;
	}

	if (Get_DIKeyState(DIK_D) & 0X80)
	{
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		
		m_pTransCom->Move_Pos(&(vRight * -10.f * fTimeDelta));
		m_eDirType = DIR_RIGHT;
	}

	if (Engine::Get_DIMouseState(DIM_LB) & 0X80) // Picking
	{
		Create_bullet(m_vPos);

		m_bOneShot = true;

		// Magazine 0 = Don't Shoot
		if (m_iMagazine == 0)
			m_bOneShot = false;


	}
	if (Get_DIKeyState(DIK_R) & 0X80)
	{
		m_iMagazine = 8;
	}




}

void CHWPlayer::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

_vec3 CHWPlayer::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

float CHWPlayer::Get_TerrainY(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	// TestTool 용
	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	// Stage 용
	/*Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);*/

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);


	return fHeight;
}

void CHWPlayer::Collsion_CubeMap(CGameObject * pGameObject)
{
	CTransform *pTrnasform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	_vec3 vCenter1Pos;
	_vec3 vPos; 

	pTrnasform->Get_Info(INFO_POS, &vCenter1Pos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	if (m_pColliderCom->Check_Sphere_InterSect(vCenter1Pos, vPos, 1.f, 1.f))
	{
		m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);
		m_pTransCom->Get_Info(INFO_UP, &m_vUp);
		m_pTransCom->Get_Info(INFO_POS, &m_vPos);
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		switch (m_eDirType)
		{
		case Engine::DIR_UP:
			m_pTransCom->Move_Pos(&(m_vDirection * -0.1f));
			break;
		case Engine::DIR_DOWN:
			m_pTransCom->Move_Pos(&(m_vDirection * 0.1f));
			break;
		case Engine::DIR_LEFT:
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			D3DXVec3Normalize(&m_vUp, &m_vUp);
			D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
			m_pTransCom->Move_Pos(&(vRight * -0.1f));
			break;
		case Engine::DIR_RIGHT:
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);
			D3DXVec3Normalize(&m_vUp, &m_vUp);
			D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
			m_pTransCom->Move_Pos(&(vRight * 0.1f ));
			break;
		case Engine::DIR_END:
			break;
		default:
			break;
		}
		m_pTransCom->Update_Component(1.f);
	}
	
	return;
}

HRESULT CHWPlayer::Create_bullet(_vec3 vPos)
{
	++m_iCoolTime;

	if (m_bOneShot && m_iCoolTime > 10)
	{
		m_bOneShot = FALSE;

		m_iCoolTime = 0;

		CScene* pScene = ::Get_Scene();
		CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

		CGameObject* pGameObject = nullptr;
		pGameObject = CObjectMgr::GetInstance()->Reuse_PlayerBulltObj(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, );
		pMyLayer->Add_GameObjectList(pGameObject);

		m_iMagazine -= 1;

	}

	return S_OK;
}

CHWPlayer * CHWPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHWPlayer *	pInstance = new CHWPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHWPlayer::Free(void)
{
	CGameObject::Free();
}
