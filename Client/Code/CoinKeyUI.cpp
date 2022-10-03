#include "stdafx.h"
#include "..\Header\CoinKeyUI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

USING(Engine)

CCoinKeyUI::CCoinKeyUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCoinKeyUI::CCoinKeyUI(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CCoinKeyUI::~CCoinKeyUI()
{
}

HRESULT CCoinKeyUI::Ready_Object(CTestPlayer * pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CCoinKeyUI::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CCoinKeyUI::LateUpdate_Object(void)
{
	m_pTransCom->OrthoMatrix(60.f, 15.f, -193.f, -230.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();
}

void CCoinKeyUI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CCoinKeyUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Coin_KeyUI_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CCoinKeyUI * CCoinKeyUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTestPlayer * pPlayer)
{
	CCoinKeyUI*	pInstance = new CCoinKeyUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPlayer)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCoinKeyUI::Free()
{
	CGameObject::Free();
}