#include "stdafx.h"
#include "..\Header\Box.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Coin.h"
#include "HealthPotion.h"
#include "Player.h"

USING(Engine)

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBox::~CBox()
{
}

HRESULT CBox::Ready_Object(_uint iX, _uint iY, CGameObject* pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(iX, 1.f, iY);
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(1, 0, 0.2f, 3); // 애니메이션이 자동으로 돌아가는 함수. 바꿔줘야 한다.

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CBox::Update_Object(const _float & fTimeDelta)
{
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Set_OnTerrain();


	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CBox::LateUpdate_Object(void)
{

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
	// ★ // 10~04 고치기
	//if (m_pColliderCom->Check_Sphere_InterSect(vObjPos, vPlayerPos, 1.f, 1.f) == true && static_cast<CPlayer*>(m_pPlayer)->Get_BoxOpen() == true)
	//{
	//	_vec3 vPos;

	//	m_pTransCom->Get_Info(INFO_POS, &vPos);

	//	// HealthPotion
	//	CGameObject* pGameObj = nullptr;
	//	
	//	pGameObj = CHealthPotion::Create(m_pGraphicDev, vPos.x - 1, vPos.z - 5);
	//	NULL_CHECK_RETURN(pGameObj, E_FAIL);
	//	FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(L"HealthPotion", pGameObj), E_FAIL);

		return S_OK;
	//	
	//}

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



CBox * CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY, CGameObject* pPlayer)
{
	CBox*	pInstance = new CBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY, pPlayer)))
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
