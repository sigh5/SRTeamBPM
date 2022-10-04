#include "stdafx.h"
#include "..\Header\Gun_Screen.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

CGun_Screen::CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGun_Screen::~CGun_Screen()
{
}

HRESULT CGun_Screen::Ready_Object(CGameObject* pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matCameraView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matCameraView);

	m_pDynamicTransCom->Set_Pos(m_matCameraView._41 - 100, m_matCameraView._42 - 100, m_matCameraView._43 - 100);

	m_pAnimationCom->Ready_Animation(4, 0, 0.2f);

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CGun_Screen::Update_Object(const _float & fTimeDelta)
{
	_uint iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	

	Add_RenderGroup(RENDER_ALPHA, this);

	return iResult;
}

void CGun_Screen::LateUpdate_Object(void)
{
}

void CGun_Screen::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matCameraView);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(0);// m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CGun_Screen::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pDynamicTransCom = CAbstractFactory<CDynamic_Transform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Gun_ScreenTexture", m_mapComponent, ID_STATIC);

	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

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
