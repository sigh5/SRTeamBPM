#include "stdafx.h"
#include "..\Header\Bullet_UI.h"
#include "Export_Function.h"
#include "HWPlayer.h"


USING(Engine)

CBullet_UI::CBullet_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBullet_UI::CBullet_UI(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CBullet_UI::~CBullet_UI()
{
}

HRESULT CBullet_UI::Ready_Object(CGameObject* pPlayer)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayer = pPlayer;
		
	return S_OK;
}

_int CBullet_UI::Update_Object(const _float & fTimeDelta)
{			
												// 여기만 바꾸면
	m_pAnimationCom->Control_Animation(dynamic_cast<CHWPlayer*>(m_pPlayer)->Get_Magazine());
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);                     

	return 0;
}

void CBullet_UI::LateUpdate_Object(void)
{
	m_pTransCom->OrthoMatrix(70.f, 20.f, 300.f, -270.f, WINCX, WINCY);

	CGameObject::LateUpdate_Object();
}

void CBullet_UI::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransCom->m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_pTransCom->m_matOrtho);


	_uint iMagazineCount = dynamic_cast<CHWPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"TestPlayer"))->Get_Magazine();
	
	// Player's Bullet Magazine left
	
	_tchar	tMagazine[MAX_PATH];
	swprintf_s(tMagazine, L"%d / 8", iMagazineCount);
	m_szMagazine = L"";
	m_szMagazine += tMagazine;

	Render_Font(L"BMYEONSUNG", m_szMagazine.c_str(), &_vec2(660.f, 520.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	// ~Player's Bullet Magazine left

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
	m_pBufferCom->Render_Buffer();
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
