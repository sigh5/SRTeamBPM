#include "stdafx.h"
#include "..\Header\MetronomeRect.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "ObjectMgr.h"


CMetronomeRect::CMetronomeRect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CMetronomeRect::~CMetronomeRect()
{
}

HRESULT CMetronomeRect::Ready_Object(_int iTexIndex)
{
	CUI_Base::Add_Component();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

 


	m_pOrthoTransCom->Set_Pos(0.f, 0.f, 0.f);

	m_iTexIndex = iTexIndex;
	return S_OK;
}

_int CMetronomeRect::Update_Object(const _float & fTimeDelta)
{
	
	m_fFrame += 1.f * fTimeDelta;

	m_fSizeX -= 40 * fTimeDelta;
	m_fSizeY -= 40 * fTimeDelta;

	m_pOrthoTransCom->OrthoMatrix(m_fSizeX, m_fSizeY, m_fMoveX, m_fMoveY, WINCX, WINCY);

	
	m_iAlpha += fTimeDelta * 10.f;

	if (m_iAlpha > 255)
		m_iAlpha = 255;

	if (m_fFrame >= 1.f)
	{
		CObjectMgr::GetInstance()->Collect_UIRect(this);
		m_fFrame = 0.f;
		m_iAlpha = 150;
		m_fSizeX = 100.f;
		m_fSizeY = 100.f;
		
		return 5;
	}


	CUI_Base::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	return 0;

}

void CMetronomeRect::LateUpdate_Object(void)
{

	CUI_Base::LateUpdate_Object();
}

void CMetronomeRect::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pOrthoTransCom->m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pOrthoTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pOrthoTransCom->m_matOrtho);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));
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

HRESULT CMetronomeRect::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ArrowTexCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CMetronomeRect * CMetronomeRect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexIndex)
{
	CMetronomeRect*	pInstance = new CMetronomeRect(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(iTexIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMetronomeRect::Free()
{
	CUI_Base::Free();
}
