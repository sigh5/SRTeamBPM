#include "stdafx.h"
#include "..\Header\MetronomeSmallUI.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "ObjectMgr.h"



CMetronomeSmallUI::CMetronomeSmallUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CMetronomeSmallUI::CMetronomeSmallUI(const CUI_Base & rhs)
	: CUI_Base(rhs)
{

}


CMetronomeSmallUI::~CMetronomeSmallUI()
{
}

HRESULT CMetronomeSmallUI::Ready_Object(_float fPosX, _float fPosY, _float fSpeed, int iTexIndex)
{

	CUI_Base::Add_Component();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fMoveX = fPosX;
	m_fMoveY = fPosY;
	m_fSpeed = fSpeed;
	m_iTexIndex = iTexIndex;

	
	return S_OK;
}

_int CMetronomeSmallUI::Update_Object(const _float & fTimeDelta)
{
	m_pOrthoTransCom->Set_Scale(&_vec3(2.f, 2.f, 2.f));

	if (m_fAlpha >= 245)
		m_fAlpha = 240;

	m_fFrame += 1 * fTimeDelta;
	m_fAlpha += 40 * fTimeDelta;

	m_pOrthoTransCom->OrthoMatrix(100.f, 100.f, m_fMoveX, m_fMoveY, WINCX, WINCY);

	if (m_fMoveX < 3.f && m_fMoveX > -3.f)	// m_fMove.x < 10.f && m_fMove.x >-10.f
	{
		// Create 큰사각형
		CObjectMgr::GetInstance()->Collect_UISmallObj(this);
		m_fAlpha = 0.f;
		return 5;
	}


	m_fMoveX += m_fSpeed * fTimeDelta;


	CUI_Base::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CMetronomeSmallUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CMetronomeSmallUI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pOrthoTransCom->m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pOrthoTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pOrthoTransCom->m_matOrtho);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((_int)m_fAlpha, 0, 0, 0));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CMetronomeSmallUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ArrowTexCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CMetronomeSmallUI * CMetronomeSmallUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fPosX, _float fPosY, _float fSpeed, int iTexIndex)
{
	CMetronomeSmallUI*	pInstance = new CMetronomeSmallUI(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(fPosX, fPosY, fSpeed, iTexIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CMetronomeSmallUI::Free()
{
	CUI_Base::Free();
}
