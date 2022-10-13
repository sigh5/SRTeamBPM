#include "stdafx.h"
#include "..\Header\ControlRoom.h"

#include "Export_Function.h"
#include "Terrain.h"

CControlRoom::CControlRoom(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CControlRoom::~CControlRoom()
{
}

HRESULT CControlRoom::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	vScale = { 10.f,10.f,10.f };

	m_pTransCom->Set_Scale(&vScale);
	
	
	//vScale = m_pTransCom->Get_Scale();
	m_pColliderCom->Set_HitRadiuos(2.f);
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	return S_OK;
}

_int CControlRoom::Update_Object(const _float & fTimeDelta)
{
	m_pColliderCom->Set_HitBoxMatrix(&(m_pTransCom->m_matWorld));


	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CControlRoom::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
	//m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

void CControlRoom::Collision_Event()
{
	// 거리안에있는 몬스터들 다체킹하는 로직
}

HRESULT CControlRoom::Add_Component(void)
{
	ADD_CLONE_PROTO(CSphereTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_SphereTexCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");
	ADD_CLONE_PROTO(CCalculator, m_pCalculatorCom, m_mapComponent, ID_STATIC, L"Proto_CalculatorCom");
	ADD_CLONE_PROTO(CCollider, m_pColliderCom, m_mapComponent, ID_STATIC, L"Proto_ColliderCom");
	


	return S_OK;
}

CControlRoom * CControlRoom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CControlRoom *	pInstance = new CControlRoom(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
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
