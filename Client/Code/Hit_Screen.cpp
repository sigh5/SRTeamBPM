#include "stdafx.h"
#include "..\Header\Hit_Screen.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player.h"
#include "Player_Dead_UI.h"


CHit_Screen::CHit_Screen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CHit_Screen::CHit_Screen(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CHit_Screen::~CHit_Screen()
{
}

HRESULT CHit_Screen::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(1280.f, 1024.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX, m_fY, 0.1f);

	m_fTime = 0.f;

	return S_OK;
}

_int CHit_Screen::Update_Object(const _float & fTimeDelta)
{
	if (m_bRender)
	{
		m_fTime += 0.2f * fTimeDelta;

		if (m_fTime >= 0.2f)
		{
			m_fTime = 0.f;
			m_bRender = false;
		}
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CHit_Screen::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CHit_Screen::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	if (pDead_UI->Get_Render() == false && m_bRender)
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

			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SUBTRACT);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR,
				D3DCOLOR_ARGB(140, 140, 140, 140));

			m_pTextureCom->Set_Texture(0);

			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		
	}
}

HRESULT CHit_Screen::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BloodScreen_Texture", m_mapComponent, ID_STATIC);

	return S_OK;
}

CHit_Screen * CHit_Screen::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHit_Screen* pInstance = new CHit_Screen(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHit_Screen::Free(void)
{
	Engine::CGameObject::Free();
}
