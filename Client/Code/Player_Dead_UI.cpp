#include "stdafx.h"
#include "..\Header\Player_Dead_UI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player.h"


CPlayer_Dead_UI::CPlayer_Dead_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CPlayer_Dead_UI::~CPlayer_Dead_UI()
{
}

HRESULT CPlayer_Dead_UI::Ready_Object(_uint _iAlpha)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(WINCX, WINCY, 0.f, 0.f);

	m_vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);

	m_pTransCom->Set_Pos(0.f, 0.f, 0.05f);

	m_iAlpha = _iAlpha;

	return S_OK;
}

_int CPlayer_Dead_UI::Update_Object(const _float & fTimeDelta)
{		
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CPlayer_Dead_UI::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CPlayer_Dead_UI::Render_Obejct(void)
{
	if (m_bRender)
	{		
		if (!m_bEvent)
		{
			--m_iAlpha;
		}
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

			// 페이드 인, 페이드 아웃
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SUBTRACT);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR,
				D3DCOLOR_ARGB(m_iAlpha, m_iAlpha, m_iAlpha, m_iAlpha));

			if (m_iAlpha == 0)
			{
				m_bEvent = true;

				_tchar szReStart[128] = L"다시 시작하시겠습니까? M키";

				Render_Font(L"Font_Jinji", szReStart, &_vec2(350.f, 800.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			}

			if (Engine::Key_Down(DIK_M))
			{
				m_bRender = false;
				m_bRenderIn = true;
			}
			::Engine::Key_InputReset();

			m_pTextureCom->Set_Texture(0);

			m_pBufferCom->Render_Buffer();


			m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

			m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			//D3DCOLOR_ARGB(1, m_iAlpha, m_iAlpha, m_iAlpha)
		
	}

	else
	{
		m_bEvent = false;
	}

	if (m_bRenderIn)
	{
		if(!m_bPlusCount)
		{
			++m_iAlpha;
		}
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

		// 페이드 인, 페이드 아웃
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SUBTRACT);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR,
			D3DCOLOR_ARGB(m_iAlpha, m_iAlpha, m_iAlpha, m_iAlpha));

		if (m_iAlpha == 255)
		{
			m_bPlusCount = true;
			m_bRenderIn = false;
		}

		m_pTextureCom->Set_Texture(0);

		m_pBufferCom->Render_Buffer();
		
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

	else
	{
		m_bPlusCount = false;
	}
}

HRESULT CPlayer_Dead_UI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_FadeOut_Texture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	
	return S_OK;
}

CPlayer_Dead_UI * CPlayer_Dead_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint _iAlpha)
{
	CPlayer_Dead_UI* pInstance = new CPlayer_Dead_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_iAlpha)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CPlayer_Dead_UI::Free()
{
	CGameObject::Free();
}
