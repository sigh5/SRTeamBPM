#include "stdafx.h"
#include "..\Header\HarpoonGuy.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Gun_Screen.h"
#include "Player.h"
#include "HitEffect.h"
#include "Coin.h"
#include "Key.h"
#include "Special_Effect.h"
#include "SoldierBullet.h"

CHarpoonGuy::CHarpoonGuy(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CHarpoonGuy::~CHarpoonGuy()
{
}

HRESULT CHarpoonGuy::Ready_Object(float Posx, float Posz)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CComponent* pComponent = nullptr;
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Harpoonguy_Texture", m_mapComponent, ID_STATIC);

	m_pAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Harpoonguy_Attack_Texture", m_mapComponent, ID_STATIC);

	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Harpoonguy_Death_Texture", m_mapComponent, ID_STATIC);

	m_pAttackAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = MONSTER_HARPOONGUY;
	m_pInfoCom->Ready_CharacterInfo(1, 10, 4.f);
	m_fAttackDelay = 0.4f;
	m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_pAttackAnimationCom->Ready_Animation(12, 0, 0.2f);
	m_pDeadAnimationCom->Ready_Animation(8, 0, 0.2f);


	m_pDynamicTransCom->Set_Scale(&_vec3(3.f, 3.f, 3.f));
	if (Posx == 0 && Posz == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((_float)Posx, m_pDynamicTransCom->m_vScale.y * 0.5f, (_float)Posz);
	}
	Save_OriginPos();

	_vec3 vPos, vScale;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	return S_OK;
}

_int CHarpoonGuy::Update_Object(const _float & fTimeDelta)
{
	// 맨위에있어야됌 리턴되면 안됌
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	// 맨위에있어야됌 리턴되면 안됌

	m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y * 0.5f);
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);

	if (Distance_Over())
	{
		m_pAnimationCom->m_iMotion = 0;
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}
	if (Dead_Judge(fTimeDelta))
	{
		return 0;
	}

	Hit_Delay_toZero();

	AttackJudge(fTimeDelta);

	if (m_bHit == false)
	{
		NoHit_Loop(fTimeDelta);
	}
	else
	{
		Hit_Loop(fTimeDelta);
	}


	m_pDynamicTransCom->Update_Component(fTimeDelta);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CHarpoonGuy::LateUpdate_Object(void)
{
	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);

	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pDynamicTransCom->m_vScale.x, m_pDynamicTransCom->m_vScale.y, m_pDynamicTransCom->m_vScale.z);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));

	// 빌보드 에러 해결
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
	pMyLayer->Add_vecColliderMonster(static_cast<CMonsterBase*>(this));

	Engine::CMonsterBase::LateUpdate_Object();
}

void CHarpoonGuy::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (false == m_bDead)
	{
		if (m_bAttacking && false == m_bHit)
		{
			m_pAttackTextureCom->Set_Texture(m_pAttackAnimationCom->m_iMotion);
		}
		else
		{
			m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
		}
	}
	else
	{
		m_pDeadTextureCom->Set_Texture(m_pDeadAnimationCom->m_iMotion);
	}
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CHarpoonGuy::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));
	_vec3	vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() &&
		fMtoPDistance < MAX_CROSSROAD  &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(1);
		cout << "Spider " << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);

		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_SPIDER, 0, 7, 0.2f);

		if (false == m_bDead)
		{
			_int Hitsound = rand() % 3;
			switch (Hitsound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Soldier_Pain_01.wav", SOUND_MONSTER, 0.4f);
				break;

			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Soldier_Pain_02.wav", SOUND_MONSTER, 0.4f);
				break;

			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Soldier_Pain_03.wav", SOUND_MONSTER, 0.4f);
				break;
			}
		}
	}
}

void CHarpoonGuy::Excution_Event()
{
	if (!m_bDead && 1 >= m_pInfoCom->Get_Hp())
	{
		m_pInfoCom->Receive_Damage(1);
		_vec3	vPos;
		CGameObject *pGameObject = nullptr;
		CScene* pScene = Get_Scene();
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		READY_CREATE_EFFECT_VECTOR(pGameObject, CSpecial_Effect, pLayer, m_pGraphicDev, vPos);
		static_cast<CSpecial_Effect*>(pGameObject)->Set_Effect_INFO(OWNER_PALYER, 0, 17, 0.2f);

		::PlaySoundW(L"explosion_1.wav", SOUND_EFFECT, 0.05f); // BGM

	}
}

bool CHarpoonGuy::Dead_Judge(const _float & fTimeDelta)
{
	if (0 >= m_pInfoCom->Get_Hp())
	{
		if (false == m_bDead)
		{
			_int Hitsound = rand() % 3;
			switch (Hitsound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Soldier_Deth_01.wav", SOUND_MONSTER, 0.4f);
				break;
			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Soldier_Deth_02.wav", SOUND_MONSTER, 0.4f);
				break;
			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Soldier_Deth_03.wav", SOUND_MONSTER, 0.4f);
				break;
			}
			Drop_Item(rand() % 3);
			m_bDead = true;
		}
	}
	if (m_bDead)
	{
		if (m_pDeadAnimationCom->m_iMotion<m_pDeadAnimationCom->m_iMaxMotion)
			m_pDeadAnimationCom->Move_Animation(fTimeDelta);
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);
		return true;
	}
	else
	{
		return false;
	}
}

void CHarpoonGuy::NoHit_Loop(const _float & fTimeDelta)
{
	if (fMtoPDistance > 10.f && m_bAttacking == false)
	{
		m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);

		m_pAnimationCom->Move_Animation(fTimeDelta);
	}
	else
	{
		//공격
		if (m_bAttack)
		{
			Attack(fTimeDelta);
		}
		else
		{
			m_pAnimationCom->m_iMotion = 0;
		}
	}
}

void CHarpoonGuy::Hit_Loop(const _float & fTimeDelta)
{
	m_pAnimationCom->m_iMotion = 7;

	m_fHitDelay += fTimeDelta;
	if (m_fHitDelay > 1.5f)
	{
		m_bHit = false;
		m_fHitDelay = 0.f;
	}
}

void CHarpoonGuy::AttackJudge(const _float & fTimeDelta)
{
	if (m_bAttack == false)
	{
		m_fAttackDelayTime += fTimeDelta;
		if (m_fAttackDelay <= m_fAttackDelayTime)
		{
			m_bAttack = true;
			m_fAttackDelayTime = 0.f;
		}
	}

	if (m_pAttackAnimationCom->m_iMotion<m_pAttackAnimationCom->m_iMaxMotion
		&& m_pAttackAnimationCom->m_iMotion>m_pAttackAnimationCom->m_iMinMotion)
	{
		m_bAttacking = true;
	}
	else
	{
		m_bAttacking = false;
	}
}

void CHarpoonGuy::Attack(const _float & fTimeDelta)
{
	m_pAttackAnimationCom->Move_Animation(fTimeDelta);
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	if (5 == m_pAttackAnimationCom->m_iMotion && false == m_bShotBullet)
	{
		//총알 만들기
		CGameObject* pBullet;
		pBullet = CSoldierBullet::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS]);
		pMyLayer->Add_EffectList(pBullet);
		m_bShotBullet = true;

		::StopSound(SOUND_EFFECT2);
		::PlaySoundW(L"LaserGun.wav", SOUND_EFFECT2, 0.4f);
	}
	if (m_pAttackAnimationCom->m_iMotion >= m_pAttackAnimationCom->m_iMaxMotion)
	{
		m_bAttack = false;
		m_bAttackSound = false;
		m_bAttackSound2 = false;
		m_bShotBullet = false;
	}
}

void CHarpoonGuy::Drop_Item(int ItemType)
{
	CScene  *pScene = ::Get_Scene();
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject* pItem = nullptr;
	switch (ItemType)
	{
	case 0:
		pItem = CCoin::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS].x, m_pDynamicTransCom->m_vInfo[INFO_POS].z);
		pLayer->Add_DropItemList(pItem);
		break;

	case 1:
		pItem = CKey::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS].x, m_pDynamicTransCom->m_vInfo[INFO_POS].z);
		pLayer->Add_DropItemList(pItem);
		break;

	default:
		break;
	}
}

CHarpoonGuy * CHarpoonGuy::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posz)
{
	CHarpoonGuy*	pInstance = new CHarpoonGuy(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posz)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHarpoonGuy::Free(void)
{
	CMonsterBase::Free();
}
