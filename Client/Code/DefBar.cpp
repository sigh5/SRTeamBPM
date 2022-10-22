#include "stdafx.h"
#include "..\Header\DefBar.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player.h"
#include "Player_Dead_UI.h"
#include "ShopUI.h"

#include "Helmet.h"

CDefBar::CDefBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CDefBar::CDefBar(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CDefBar::~CDefBar()
{
}

HRESULT CDefBar::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 0.85f, m_fSizeY * 0.9f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX - 375.f, m_fY - 395.f, 0.1f);

	m_iPlayerDef = 0;

	m_pAnimationCom->Ready_Animation(10, 0, 0.2f, 10);

	return S_OK;
}

_int CDefBar::Update_Object(const _float & fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	m_iPlayerDef = (pPlayer->Get_DefChange()) / 10;
									// m_iPlayerDef
	m_pAnimationCom->Control_Animation(m_iPlayerDef);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CDefBar::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CDefBar::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));
	CHelmet* pHelmet = static_cast<CHelmet*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Helmet1"));

	if (pHelmet->Get_EquipCheck() == true)
	{
		if (pDead_UI->Get_Render() == false)
		{
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
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

HRESULT CDefBar::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_DefBar_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CDefBar * CDefBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefBar*	pInstance = new CDefBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CDefBar::Free()
{
	Engine::CGameObject::Free();
}
