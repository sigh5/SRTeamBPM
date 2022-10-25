#include "stdafx.h"
#include "..\Header\MsgUI.h"

#include "Export_Function.h"
#include "AbstractFactory.h"



CMsgUI::CMsgUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CMsgUI::~CMsgUI()
{
}

HRESULT CMsgUI::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fSizeX = 600.f;
	m_fSizeY = 400.f;

	m_fX = WINCX / 2.f;
	m_fY = WINCY / 2.f;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);

	return S_OK;
}

_int CMsgUI::Update_Object(const _float & fTimeDelta)
{
	
	if (m_bSetActive)
	{
		m_bActiveTimer += 1.f*fTimeDelta;
	}

	if (m_bActiveTimer >= 1.5f)
	{
		m_bActiveTimer = 0.f;
		m_bSetActive = false;
	}
		
	
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CMsgUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CMsgUI::Render_Obejct(void)
{
	if (!m_bSetActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	_matrix		OldViewMatrix, OldProjMatrix;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	_matrix		ViewMatrix;
	ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	//
	Render_Font(L"DalseoHealingBold", pString.c_str(), &_vec2(m_fSetWidth, m_fSetHeight), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CMsgUI::Set_FontMsg(wstring pStr, _float PosX, _float PosY)
{
	pString = pStr;
	m_fSetWidth = PosX;
	m_fSetHeight = PosY;
}

void CMsgUI::Set_MsgFrame(_float fx, _float fy, _float fSizeX, _float fSizxY)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizxY;

	m_fX = fx; // WINCX / 2.f + 500;
	m_fY = fy; //WINCY / 2.f - 150;

	_vec3 vScale = { m_fSizeX, m_fSizeY, 1.f };
	m_pTransCom->Set_Scale(&vScale);
	m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f,
		(-m_fY + WINCY * 0.5f), 0.0f);

	m_pTransCom->Update_Component(1.f);
}

HRESULT CMsgUI::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_LoadingFontBackGround_Texture", m_mapComponent, ID_STATIC);

	return S_OK;
}

CMsgUI * CMsgUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMsgUI* pInstance = new CMsgUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CMsgUI::Free()
{

	CGameObject::Free();
}
