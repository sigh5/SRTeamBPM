#include "stdafx.h"
#include "..\Header\HpBar.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Player.h"

USING(Engine)

CHpBar::CHpBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CHpBar::CHpBar(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CHpBar::~CHpBar()
{
}


HRESULT CHpBar::Ready_Object(CGameObject * pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CHpBar::Update_Object(const _float & fTimeDelta)
{
	CTestPlayer* pPlayer = static_cast<CTestPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

	_uint iA = (pPlayer->Get_HpChange()) / 25;

	m_pAnimationCom->Control_Animation(iA);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CHpBar::LateUpdate_Object(void)
{
	m_pTransCom->OrthoMatrix(200.f, 154.f, -310.f, -268.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();

}

void CHpBar::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
	m_pBufferCom->Render_Buffer();
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
	m_pAnimationCom->Ready_Animation(4, 0, 0.2f, 3); // 8
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	return S_OK;
}


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
