#include "stdafx.h"
#include "..\Header\HpBar.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "Player_Dead_UI.h"
#include "CharacterInfo.h"
#include "ShopUI.h"

USING(Engine)

CHpBar::CHpBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CHpBar::CHpBar(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CHpBar::~CHpBar()
{
}


HRESULT CHpBar::Ready_Object(CGameObject * pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 0.85f, m_fSizeY * 0.9f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX - 375.f, m_fY - 345.f, 0.1f);

	m_pPlayer = pPlayer;

	m_iPlayerHp = 0;

	m_iHpFont = 0;

	m_pAnimationCom->Ready_Animation(10, 0, 0.2f, 10);

	return S_OK;
}

_int CHpBar::Update_Object(const _float & fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	
	m_iPlayerHp = (pPlayer->Get_HpChange()) / 10;
				// 24/25 = 0.96 // 0/25 = 0

	//m_iHpFont = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._iHp;

	/*if (m_pAnimationCom->m_iMotion == 4)
	{
		if (Engine::Key_Down(DIK_M)) 
		{
			m_pAnimationCom->m_iMotion = 0;
		}
		Engine::Key_InputReset();
	}*/ 

	m_pAnimationCom->Control_Animation(m_iPlayerHp);
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CHpBar::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CHpBar::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));
	CShopUI* pShopUI = static_cast<CShopUI*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShopUI"));
	CCharacterInfo* pInfo = dynamic_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));

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
	
		if(pInfo->Get_InfoRef()._iDefense == 0)
		m_iHpFont = dynamic_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC))->Get_InfoRef()._iHp;

		//cout << "ÆùÆ® : " << iHpFont << endl;

		_tchar	tPlayerHp[MAX_PATH];
		swprintf_s(tPlayerHp, L"%d / 100", m_iHpFont);
		m_szPlayerHp = L"";
		m_szPlayerHp += tPlayerHp;

		if (pShopUI->Get_Active() == false)
		Render_Font(L"DalseoHealingBold", m_szPlayerHp.c_str(), &_vec2(190.f, 950.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

		//cout << m_pAnimationCom->m_iMotion << endl;

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

HRESULT CHpBar::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_HpBar_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HpBar_Texture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<COrthoTransform*>(Clone_Proto(L"Proto_OrthoTransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_OrthoTransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	//m_pAnimationCom->Ready_Animation(5, 0, 0.2f, 4); // 8
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	return S_OK;
}

//void CHpBar::Render_PlayerHpFont(void)
//{
//	_tchar	tPlayerHp[MAX_PATH];
//	swprintf_s(tPlayerHp, L"%d", m_iHpFont);
//	m_szPlayerHp += tPlayerHp;
//
//	Render_Font(L"HoengseongHanu", m_szPlayerHp.c_str(), &_vec2(275.f, 960.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
//
//}


CHpBar * CHpBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject * pPlayer)
{
	CHpBar*	pInstance = new CHpBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPlayer)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHpBar::Free(void)
{
	CGameObject::Free();
}
