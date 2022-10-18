#include "stdafx.h"
#include "..\Header\UI_Effect.h"

#include "Export_Function.h"
#include "AbstractFactory.h"

CUI_Effect::CUI_Effect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI_Base(pGraphicDev)
{
}

CUI_Effect::~CUI_Effect()
{
}

HRESULT CUI_Effect::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { WINCX , WINCY , 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX , m_fY , 0.1f);

	m_fDelay = 15.f;
	m_pAnimationCom->Ready_Animation(4, 0, 0.05f);
	return S_OK;
}

_int CUI_Effect::Update_Object(const _float & fTimeDelta)
{
	if (m_bActive)
	{
		m_pAnimationCom->Move_Animation(fTimeDelta);
	}

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CUI_Effect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CUI_Effect::Render_Obejct(void)
{
	if (!m_bActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		OldViewMatrix, OldProjMatrix;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	_matrix		ViewMatrix;

	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

	_matrix		matProj;

	Get_ProjMatrix(&matProj);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xDF);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}


HRESULT CUI_Effect::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_DashingEffectTexture", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CUI_Effect * CUI_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CUI_Effect* pInstance = new CUI_Effect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUI_Effect::Free(void)
{
	CGameObject::Free();
}
