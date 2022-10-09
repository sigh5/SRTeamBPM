#include "stdafx.h"
#include "..\Header\Weapon_UI.h"

#include "Export_Function.h"
#include "Player.h"

#include "ShotGun.h"
#include "Magnum.h"

USING(Engine)


CWeapon_UI::CWeapon_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CWeapon_UI::CWeapon_UI(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CWeapon_UI::~CWeapon_UI()
{
}

HRESULT CWeapon_UI::Ready_Object(CGameObject * pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayer = pPlayer;

	return S_OK;
}

_int CWeapon_UI::Update_Object(const _float & fTimeDelta)
{
	//m_pAnimationCom->Control_Animation(static_cast<CPlayer*>(m_pPlayer)->Get_ChangeImage());

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ICON, this);

	return 0;
}

void CWeapon_UI::LateUpdate_Object(void)
{
	m_pTransCom->OrthoMatrix(110.f, 40.f, 336.f, -190.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();
}

void CWeapon_UI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x01);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	/*if (dynamic_cast<CShotGun*>(Engine::Get_GameObject(L"Layer_GameLogic", L"ShotGun"))->Get_RenderFalse() == true)
	{
		m_pTextureCom->Set_Texture(1);
	}

	else*/
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CWeapon_UI::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_WeaponUI_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_WeaponUI_Texture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<COrthoTransform*>(Clone_Proto(L"Proto_OrthoTransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_OrthoTransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	//pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	//NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	//m_pAnimationCom->Ready_Animation(2, 0, 0.2f, 1); // 8
	//m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	return S_OK;
}

CWeapon_UI * CWeapon_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject * pPlayer)
{
	CWeapon_UI* pInstance = new CWeapon_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPlayer)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CWeapon_UI::Free()
{
	CGameObject::Free();
}
