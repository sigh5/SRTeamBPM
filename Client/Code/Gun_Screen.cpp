#include "stdafx.h"
#include "..\Header\Gun_Screen.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "TestPlayer.h"


USING(Engine)

CGun_Screen::CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGun_Screen::CGun_Screen(const CGun_Screen & rhs)
	: CGameObject(rhs)
{
}

CGun_Screen::~CGun_Screen()
{
}

HRESULT CGun_Screen::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pAnimationCom->Ready_Animation(4, 0, 0.2f);

	m_bShootCheck = false;

	return S_OK;
}

_int CGun_Screen::Update_Object(const _float & fTimeDelta)
{

	Engine::CGameObject::Update_Object(fTimeDelta);
	
	
	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CGun_Screen::LateUpdate_Object(void)
{
	Shoot_Motion();

	m_pOrthoTransCom->OrthoMatrix(280.f, 230.f, 50.f, -150.f, WINCX, WINCY);

	
	/*CPlayer* pPlayer = static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic",L"Player"));

	if (m_bShootCheck)
	{
		pPlayer->Reset_ComboCount();
	}

	m_bShootCheck = false;*/

	CGameObject::LateUpdate_Object();
	

	

}

void CGun_Screen::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pOrthoTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pOrthoTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pOrthoTransCom->m_matOrtho);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
}

HRESULT CGun_Screen::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pOrthoTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Gun_ScreenTexture", m_mapComponent, ID_STATIC);
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
