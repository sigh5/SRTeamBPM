#include "stdafx.h"
#include "..\Header\EndingImage.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player_Dead_UI.h"

CEndingImage::CEndingImage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CEndingImage::CEndingImage(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CEndingImage::~CEndingImage()
{
}

HRESULT CEndingImage::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(1280.f, 1024.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX, m_fSizeY, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX, m_fY, 0.03f);

	return S_OK;
}

_int CEndingImage::Update_Object(const _float & fTimeDelta)
{
	if (m_bRender)
	{
		Engine::PlaySoundW(L"GTA-V-Welcome-to-Los-Santosx.mp3", SOUND_ENDINGSCENE, 1.f);
	}

	/*if (m_fCount > 200.f)
		--m_fCount;

	else
		m_fCount = 200.f;*/

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CEndingImage::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CEndingImage::Render_Obejct(void)
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
	
		//Render_Font(L"BMJUA_ttf", L"SR TEAM PORTFOLIO", &_vec2(640.f, m_fCount), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		
		m_pTextureCom->Set_Texture(0);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

HRESULT CEndingImage::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_EndingTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CEndingImage * CEndingImage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEndingImage*	pInstance = new CEndingImage(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEndingImage::Free(void)
{
	CGameObject::Free();
}
