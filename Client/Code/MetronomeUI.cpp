#include "stdafx.h"
#include "..\Header\MetronomeUI.h"

#include "Export_Function.h"
#include "AbstractFactory.h"



CMetronomeUI::CMetronomeUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMetronomeUI::CMetronomeUI(const CGameObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CMetronomeUI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMetronomeUI::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CMetronomeUI::LateUpdate_Object(void)
{
	m_pTransCom->OrthoMatrix(50.f, 50.f, 200.f, 200.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();
}

void CMetronomeUI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CMetronomeUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_UItexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CMetronomeUI * CMetronomeUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMetronomeUI*	pInstance = new CMetronomeUI(pGraphicDev);


	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMetronomeUI::Free()
{
	CGameObject::Free();
}
