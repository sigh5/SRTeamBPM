#include "stdafx.h"
#include "..\Header\Bullet_UI.h"
#include "Export_Function.h"

#include "Player.h"
#include "Gun_Screen.h"
#include "Player_Dead_UI.h"
#include "ShotGun.h"
#include "ShopUI.h"

// Work


USING(Engine)

CBullet_UI::CBullet_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CBullet_UI::CBullet_UI(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CBullet_UI::~CBullet_UI()
{
}

HRESULT CBullet_UI::Ready_Object(CGameObject* pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 0.5f, m_fSizeY * 0.7f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX + 514.f, m_fY - WINCY * 0.4f, 0.1f);
		
	return S_OK;
}

_int CBullet_UI::Update_Object(const _float & fTimeDelta)
{	
	CGun_Screen* pGun_Screen = static_cast<CGun_Screen*>(Engine::Get_GameObject(L"Layer_UI", L"Gun"));
	CShotGun* pShotGun = static_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"));
	
	if (pGun_Screen->Get_miID() == ID_MAGNUM && !m_bMagnum)
	{
		m_pAnimationCom->Set_Motion(8);
		m_pAnimationCom->Set_MaxMotion(8);
		m_bMagnum = true;
		m_bShotgun = false;
	}

	else if (pShotGun != nullptr && pGun_Screen->Get_miID() == ID_SHOT_GUN && !m_bShotgun)
	{
		m_pAnimationCom->Set_Motion(6);
		m_pAnimationCom->Set_MaxMotion(6);
		m_bShotgun = true;
		m_bMagnum = false;
	}	

	m_pAnimationCom->Control_Animation(dynamic_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"))->Get_Magazine());
		
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CBullet_UI::LateUpdate_Object(void)
{
	//m_pTransCom->OrthoMatrix(150.f, 250.f, 520.f, -400.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();
}

void CBullet_UI::Render_Obejct(void)
{
	CPlayer_Dead_UI* pDead_UI = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));
	CGun_Screen* pGun_Screen = static_cast<CGun_Screen*>(Engine::Get_GameObject(L"Layer_UI", L"Gun"));
	CShotGun* pShotGun = static_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"));
	CShopUI* pShopUI = static_cast<CShopUI*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShopUI"));

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

		_uint iMagazineCount = dynamic_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"))->Get_Magazine();

		// Player's Bullet Magazine left

		_tchar	tMagazine[MAX_PATH];

		if (pGun_Screen->Get_miID() == ID_MAGNUM)
		{
			swprintf_s(tMagazine, L"%d / 8", iMagazineCount);
		}

		else if (pShotGun != nullptr && pGun_Screen->Get_miID() == ID_SHOT_GUN)
		{
			swprintf_s(tMagazine, L"%d / 6", iMagazineCount);
		}

		m_szMagazine = L"";
		m_szMagazine += tMagazine;

		if(pShopUI->Get_Active() == false)
		Render_Font(L"HoengseongHanu", m_szMagazine.c_str(), &_vec2(1112.f, 810.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		
		_uint  iComboCount = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"))->Get_ComboCount();
		if (iComboCount != 0)
		{
			_tchar	tCobmoCount[MAX_PATH];
			swprintf_s(tCobmoCount, L"%d", iComboCount);
			m_szComboCount = L"";
			m_szComboCount += tCobmoCount;
			Render_Font(L"BMYEONSUNG", L"COMBO", &_vec2(1080.f, 50.f), D3DXCOLOR(1.f, 255.f, 1.f, 1.f));
			Render_Font(L"LeeSoonSin", m_szComboCount.c_str(), &_vec2(1120.f, 90.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


		}
		// ~Player's Bullet Magazine left

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
		
		if (pShotGun != nullptr && pGun_Screen->Get_miID() == ID_SHOT_GUN)
			m_pSG_TextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

HRESULT CBullet_UI::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_BulletUI_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_BulletUI_Texture", pComponent });

	pComponent = m_pSG_TextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_ShotGunShellUI_Texture"));
	NULL_CHECK_RETURN(m_pSG_TextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ShotGunShellUI_Texture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<COrthoTransform*>(Clone_Proto(L"Proto_OrthoTransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_OrthoTransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	m_pAnimationCom->Ready_Animation(8, 0, 0.2f, 8); // 8
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	return S_OK;
}

CBullet_UI * CBullet_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer)
{
	CBullet_UI* pInstance = new CBullet_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPlayer)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CBullet_UI::Free()
{
	CGameObject::Free();
}
