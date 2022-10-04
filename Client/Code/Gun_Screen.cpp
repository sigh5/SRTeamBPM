#include "stdafx.h"
#include "..\Header\Gun_Screen.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

USING(Engine)

CGun_Screen::CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_bShootAnimation(false)
{
}

CGun_Screen::CGun_Screen(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CGun_Screen::~CGun_Screen()
{
}

HRESULT CGun_Screen::Ready_Object(CGameObject* pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pAnimationCom->Ready_Animation(4, 0, 0.2f);

	m_pPlayer = pPlayer;

	m_bShootAnimation = false;

	return S_OK;
}

_int CGun_Screen::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Shoot_Motion();

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CGun_Screen::LateUpdate_Object(void)
{
	m_pOrthoTransCom->OrthoMatrix(280.f, 230.f, 50.f, -150.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();
}

void CGun_Screen::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pOrthoTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pOrthoTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pOrthoTransCom->m_matOrtho);
	
	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	
}

HRESULT CGun_Screen::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Gun_ScreenTexture", m_mapComponent, ID_STATIC);
	m_pOrthoTransCom = CAbstractFactory<COrthoTransform>::Clone_Proto_Component(L"Proto_OrthoTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	
	return S_OK;
}

HRESULT CGun_Screen::Shoot_Motion(void)
{
	if (m_bShootAnimation == true)
	{
		m_pAnimationCom->Gun_Animation(&m_bShootAnimation);
		
	}		

	return S_OK;
}

CGun_Screen * CGun_Screen::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer)
{
	CGun_Screen* pInstance = new CGun_Screen(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPlayer)))
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
