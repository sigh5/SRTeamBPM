#include "stdafx.h"
#include "..\Header\Magnum.h"
#include "Export_Function.h"

#include "AbstractFactory.h"

// Player는 기본적으로 이걸 들고 있다.

CMagnum::CMagnum(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEquipmentBase(pGraphicDev)
{
}

CMagnum::~CMagnum()
{
}

HRESULT CMagnum::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pInfoCom->Ready_EquipInfo(5, 0, 0, 7);
	m_pInfoCom->Set_WeaponType(WEAPON_MAGNUM);

	return S_OK;
}

_int CMagnum::Update_Object(const _float & fTimeDelta)
{
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Set_OnTerrain();

	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CMagnum::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CMagnum::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CMagnum::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"TestPlayer");
	NULL_CHECK_RETURN(pGameObject, );

	if (m_pColliderCom->Check_Collision(this, pGameObject, 1, 1))
	{
		if (Get_DIKeyState(DIK_F) & 0X80)
		{

		}
	}
}

void CMagnum::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight + 0.8f, vPos.z);
}

HRESULT CMagnum::Add_Component(void)
{
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ShotGunTexture", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	m_pInfoCom = CAbstractFactory<CEquipInfo>::Clone_Proto_Component(L"Proto_EquipInfoCom", m_mapComponent, ID_STATIC);


	return S_OK;
}

CMagnum * CMagnum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnum* pInstance = new CMagnum(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMagnum::Free(void)
{
	CGameObject::Free();
}
