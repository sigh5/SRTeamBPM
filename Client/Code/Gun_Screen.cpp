#include "stdafx.h"
#include "..\Header\Gun_Screen.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "TestPlayer.h"

#include "ShotGun.h"
#include "BulletShell.h"



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

	m_pAnimationCom->Ready_Animation(4, 0, 0.11f);

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


	Shoot_Motion(fTimeDelta);

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

HRESULT CGun_Screen::Shoot_Motion(const _float& fTimeDelta)
{
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	if (m_bAnimation == true)
	{
		m_bAnimation = m_pAnimationCom->Gun_Animation(fTimeDelta);
		if (false == m_bCreatedShell)
		{
			_vec3 vPos, vDir;
			Get_shellPosition(vPos, vDir);
			CGameObject* pShell = CBulletShell::Create(m_pGraphicDev, vPos, vDir);
			CScene  *pScene = ::Get_Scene();
			NULL_CHECK_RETURN(pScene, );
			CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
			pLayer->Add_GameObjectList(pShell);
			m_bCreatedShell = true;
		}
		if (false == m_bAnimation)
		{
			m_bCreatedShell = false;
		}
	}		

	return S_OK;
}

void CGun_Screen::GunFailSound()
{
	::PlaySoundW(L"Rythm_Check_Fail.wav", SOUND_EFFECT, 0.1f);
}

void CGun_Screen::Get_shellPosition(_vec3& vPos, _vec3& vdir)
{//탄피
	_vec3  vPlayerPos,vPlayerDirection;
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	CTransform* pPlayerTransform = static_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	vPlayerDirection = pPlayer->Get_Direction();
	D3DXVec3Normalize(&vPlayerDirection, &vPlayerDirection);
	vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	
	D3DXVec3Cross(&vdir, &_vec3(0.f, 1.f, 0.f), &vPlayerDirection);
	D3DXVec3Normalize(&vdir, &vdir);
	vPos = vPlayerPos + vPlayerDirection + vdir * 0.3f;
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
