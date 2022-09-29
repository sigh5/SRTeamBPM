#include "stdafx.h"
#include "..\Header\HealthPotion.h"
#include "Export_Function.h"

#include "AbstractFactory.h"

USING(Engine)

CHealthPotion::CHealthPotion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItemBase(pGraphicDev)
{
}

CHealthPotion::~CHealthPotion()
{
}

HRESULT CHealthPotion::Ready_Object(_uint iX, _uint iY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimationCom->Ready_Animation(3, 0, 0.2f); 

	if (iX == 0 && iY == 0) {}
	else
	{		
		Set_TransformPositon(g_hWnd, m_pCalculatorCom);		
	}

	/*_vec3	vecScale = { 1.0f, 0.f, 1.0f };

	m_pTransCom->Set_Scale(&vecScale);*/

	return S_OK;
}

_int CHealthPotion::Update_Object(const _float & fTimeDelta)
{	
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);
	
	m_pAnimationCom->Move_Animation(fTimeDelta);

	Set_OnTerrain();
	

	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CHealthPotion::LateUpdate_Object(void)
{
	
}

void CHealthPotion::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion); 
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CHealthPotion::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_HpPotionTexture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	
	return S_OK;
}

void CHealthPotion::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

CHealthPotion * CHealthPotion::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY)
{
	CHealthPotion* pInstance = new CHealthPotion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHealthPotion::Free(void)
{
	CGameObject::Free();
}
