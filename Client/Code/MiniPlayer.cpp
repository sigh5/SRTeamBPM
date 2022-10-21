#include "stdafx.h"
#include "..\Header\MiniPlayer.h"

#include "Export_Function.h"
#include "AbstractFactory.h"


CMiniPlayer::CMiniPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	,  m_vDirection(0.f, 0.f, 0.f)
{
}

CMiniPlayer::~CMiniPlayer()
{
}

HRESULT CMiniPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos = { 0.f, 1.f, 0.f };

	m_pDynamicTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);


	return S_OK;
}

_int CMiniPlayer::Update_Object(const _float & fTimeDelta)
{

	Key_Input(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CMiniPlayer::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CMiniPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	m_pColliderCom->Render_Buffer();
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

void CMiniPlayer::Collision_Event()
{
}

HRESULT CMiniPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_PlayerTexture", m_mapComponent, ID_STATIC);
	m_pDynamicTransCom = CAbstractFactory<CDynamic_Transform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

void CMiniPlayer::Key_Input(const _float & fTimeDelta)
{
	m_pDynamicTransCom->Get_Info(INFO_LOOK, &m_vDirection);
	m_pDynamicTransCom->Get_Info(INFO_UP, &m_vUp);
	m_pDynamicTransCom->Get_Info(INFO_POS, &m_vPos);


	if (Get_DIKeyState(DIK_W) & 0X80)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * 5.f * fTimeDelta));

	}

	if (Get_DIKeyState(DIK_S) & 0X80)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * -5.f * fTimeDelta));


	}

	if (Get_DIKeyState(DIK_A) & 0X80)
	{
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * 5.f * fTimeDelta));

	}

	if (Get_DIKeyState(DIK_D) & 0X80)
	{
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * -5.f * fTimeDelta));


	}

}

void CMiniPlayer::Set_OnTerrain()
{
	_vec3		vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainMoveTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainMoveTex*>(Engine::Get_Component(L"Layer_Environment", L"MiniTerrain3", L"Proto_TerrainMoveTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pDynamicTransCom->Set_Pos(vPos.x, fHeight, vPos.z);


}





CMiniPlayer * CMiniPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniPlayer *	pInstance = new CMiniPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniPlayer::Free(void)
{
	CGameObject::Free();
}
