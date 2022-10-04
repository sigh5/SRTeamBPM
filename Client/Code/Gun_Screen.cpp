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
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
//
//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
//
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
//
//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
//
////	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
//
//	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
//
//	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
//
//	m_pBufferCom->Render_Buffer();
//	
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
//
//	m_pGraphicDev->EndScene();
//	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
	
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
	CTestPlayer* pPlayer = dynamic_cast<CTestPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

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
