#include "stdafx.h"
#include "..\Header\Gun_Screen.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "TestPlayer.h"

#include "ShotGun.h"


USING(Engine)

CGun_Screen::CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
	D3DXVec3Normalize(&m_vecScale, &m_vecScale);
}

CGun_Screen::CGun_Screen(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}


CGun_Screen::~CGun_Screen()
{
}

HRESULT CGun_Screen::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_OrthoMatrix(300.f, 300.f, 0.f, 0.f);

	m_vecScale = { m_fSizeX * 1.6f, m_fSizeY * 2.f, 1.f };

	m_pTransCom->Set_Scale(&m_vecScale);
	m_pTransCom->Set_Pos(m_fX + 274.f, m_fY - 300.f, 0.1f);

	m_pAnimationCom->Ready_Animation(4, 0, 0.2f);

	m_bShootCheck = false;

	return S_OK;
}

_int CGun_Screen::Update_Object(const _float & fTimeDelta)
{
	if (m_bActive)
	{
		m_fActiveTimer += 1.f*fTimeDelta;
	}
	
	if (m_fActiveTimer >= 1.f)
	{
		m_bActive = false;
		m_fActiveTimer = 0.f;
	}




	Add_UpdateComponent();

	CShotGun* pShotGun = static_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"));

	if (pShotGun->Get_RenderFalse() == true)
	{
		m_pAnimationCom->Ready_Animation(15, 0, 0.2f);
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
		
	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CGun_Screen::LateUpdate_Object(void)
{
	Shoot_Motion();

	CPlayer* pPlayer = static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic",L"Player"));

	if (m_bShootCheck)
		pPlayer->Reset_ComboCount();
	
	m_bShootCheck = false;
	CGameObject::LateUpdate_Object();
}

void CGun_Screen::Render_Obejct(void)
{
	if (m_bActive || m_bNoRender)
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
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
}

HRESULT CGun_Screen::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
		
	if(!m_bChangeWeaponUI)
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Gun_ScreenTexture", m_mapComponent, ID_STATIC);

	return S_OK;
}

HRESULT CGun_Screen::Add_UpdateComponent(void)
{							// 임시 _bool 변수
	if (m_bChangeWeaponUI && !m_bControl)
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_ShotGun_ScreenTexture", m_mapComponent, ID_STATIC);

		m_bControl = true;
	}
	
	return S_OK;
}

HRESULT CGun_Screen::Shoot_Motion(void)
{
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	if (m_bAnimation == true)
	{
		m_pAnimationCom->Gun_Animation(&m_bAnimation);		
	}		

	return S_OK;
}

void CGun_Screen::GunFailSound()
{
	::PlaySoundW(L"Rythm_Check_Fail.wav", SOUND_EFFECT, 0.1f);
}

CGun_Screen * CGun_Screen::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGun_Screen* pInstance = new CGun_Screen(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
} 

void CGun_Screen::Free(void)
{	
	CGameObject::Free();
}
