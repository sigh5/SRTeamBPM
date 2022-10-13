#include "stdafx.h"
#include "..\Header\Ax.h"

#include "Export_Function.h"
#include "Player.h"
#include "AbstractFactory.h"

CAx::CAx(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CAx::CAx(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CAx::~CAx()
{
}

HRESULT CAx::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 7.f, m_fSizeY * 7.f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX + 274, m_fY+500.f, 0.1f);

	m_pAnimationCom->Ready_Animation(9, 0, 0.18f);

	return S_OK;
}

_int CAx::Update_Object(const _float & fTimeDelta)
{
	if (m_bActive)
	{
		m_fActiveTimer += 1.f*fTimeDelta;
	}

	if (m_fActiveTimer >= 2.f)
	{
		m_bActive = false;
		m_fActiveTimer = 0.f;
	}
	else if(m_fActiveTimer < 2.f && m_bActive )
	{
		m_pAnimationCom->Move_Animation(fTimeDelta);
		if (m_pAnimationCom->m_iMaxMotion <= m_pAnimationCom->m_iMotion)
		{
			m_bActive = false;
			m_fActiveTimer = 0.f;
			m_pAnimationCom->m_iMotion = 0.f;
		}

	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;

}

void CAx::LateUpdate_Object(void)
{
	/*if (m_bAnimation == true)
	{
		m_pAnimationCom->Gun_Animation(&m_bAnimation);
	}*/
	CGameObject::LateUpdate_Object();
}

void CAx::Render_Obejct(void)
{
	if (m_bActive ==false)
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
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CAx::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_AX_ScreenTexture", m_mapComponent, ID_STATIC);

	return S_OK;
}

CAx * CAx::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAx* pInstance = new CAx(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CAx::Free(void)
{
	CGameObject::Free();
}
