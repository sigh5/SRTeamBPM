#include "stdafx.h"
#include "..\Header\CoinKeyUI.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player.h"
#include "Player_Dead_UI.h"

#include "Coin.h" 
#include "ShopUI.h"
#include "EndingImage.h"

USING(Engine)

CCoinKeyUI::CCoinKeyUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CCoinKeyUI::CCoinKeyUI(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CCoinKeyUI::~CCoinKeyUI()
{
}

HRESULT CCoinKeyUI::Ready_Object(CGameObject * pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 0.5f, m_fSizeY * 0.28f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX - 459.f, m_fY - WINCY * 0.126f, 0.1f);

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CCoinKeyUI::Update_Object(const _float & fTimeDelta)
{
	m_iPlayerCoin = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._iCoin;

	m_iPlayerKey = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._iKey;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CCoinKeyUI::LateUpdate_Object(void)
{	
	CGameObject::LateUpdate_Object();
}

void CCoinKeyUI::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));
	CShopUI* pShopUI = static_cast<CShopUI*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShopUI"));
	CEndingImage* pEndingImage = static_cast<CEndingImage*>(Engine::Get_GameObject(L"Layer_UI", L"EndingImage"));

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

		/* m_szCoin, m_szKey */

		// Coin
		_tchar	tBCoin[MAX_PATH];
		swprintf_s(tBCoin, L"%d", m_iPlayerCoin);
		m_szCoin = L"";
		m_szCoin += tBCoin;

		if (pShopUI->Get_Active() == false)
		{
			if (pEndingImage->Get_Render() == false)
				Render_Font(L"HoengseongHanu", m_szCoin.c_str(), &_vec2(125.f, 680.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		// ~Coin

		// Key
		_tchar	tBKey[MAX_PATH];
		swprintf_s(tBKey, L"%d", m_iPlayerKey);
		m_szKey = L"";
		m_szKey += tBKey;

		if (pShopUI->Get_Active() == false)
		{
			if (pEndingImage->Get_Render() == false)
				Render_Font(L"HoengseongHanu", m_szKey.c_str(), &_vec2(203.f, 680.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		}
		// ~Key

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
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

CCoinKeyUI * CCoinKeyUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject * pPlayer)
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
