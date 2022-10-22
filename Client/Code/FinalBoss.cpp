#include "stdafx.h"
#include "..\Header\FinalBoss.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "HitEffect.h"
#include "Player.h"
#include "Gun_Screen.h"
#include "TapeWorm.h"
#include "HarpoonBullet.h"
#include "Tentacle.h"
#include "ThingySpike.h"
#include "FinalBossBullet.h"
#include "Flare.h"

CFinalBoss::CFinalBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CFinalBoss::~CFinalBoss()
{
}

HRESULT CFinalBoss::Ready_Object(float Posx, float Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pCamouTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Harpoonguy_Texture", m_mapComponent, ID_STATIC);
	m_pCamouAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Harpoonguy_Attack_Texture", m_mapComponent, ID_STATIC);
	m_pMorphTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_Morph_Texture", m_mapComponent, ID_STATIC);
	m_pThingy_AttackATextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_AttackA_Texture", m_mapComponent, ID_STATIC);
	m_pThingy_AttackBTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_AttackB_Texture", m_mapComponent, ID_STATIC);
	m_pThingy_AttackCTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_AttackC_Texture", m_mapComponent, ID_STATIC);
	m_pThingyTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_Texture", m_mapComponent, ID_STATIC);
	m_pThingy_OnekiokTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_Onekiok_Texture", m_mapComponent, ID_STATIC);
	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Thingy_Death_Texture", m_mapComponent, ID_STATIC);
	
	m_pThingy_Onekiok_AnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pThingy_Onekiok_AnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Onekiok_AnimationCom", m_pThingy_Onekiok_AnimationCom });

	m_pCamouAttackAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pCamouAttackAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CamouAttack_AnimationCom", m_pCamouAttackAnimationCom });

	m_pMorphAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pCamouAttackAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ThingyMorph_AnimationCom", m_pMorphAnimationCom });

	m_pThingy_AttackA_AnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pThingy_AttackA_AnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ThingyAttack_A_AnimationCom", m_pThingy_AttackA_AnimationCom });

	m_pThingy_AttackB_AnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pThingy_AttackB_AnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ThingyAttack_B_AnimationCom", m_pThingy_AttackB_AnimationCom });

	m_pThingy_AttackC_AnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pThingy_AttackC_AnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ThingyAttack_C_AnimationCom", m_pThingy_AttackC_AnimationCom });

	m_pThingy_AnimatiionCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pThingy_AnimatiionCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_AnimationCom", m_pThingy_AnimatiionCom });

	m_pDeadAnimationCom->Ready_Animation(9, 0, 0.7f);

	m_pCamouAttackAnimationCom->Ready_Animation(12, 0, 0.2f); // 하푼가이 어택

	m_pAnimationCom->Ready_Animation(6, 1, 0.2f); // 하푼가이 0번 아이들 7번 hit

	m_pMorphAnimationCom->Ready_Animation(17, 0, 0.3f); //변신

	m_pThingy_Onekiok_AnimationCom->Ready_Animation(12, 0, 0.4f);

	m_pThingy_AttackA_AnimationCom->Ready_Animation(17, 0, 0.2f); //어택A
	m_pThingy_AttackB_AnimationCom->Ready_Animation(27, 0, 0.25f); //B
	m_pThingy_AttackC_AnimationCom->Ready_Animation(13, 0, 0.15f); //C

	m_pThingy_AnimatiionCom->Ready_Animation(6, 1, 0.35f);

	m_vMonsterRight = _vec3(0.f, 0.f, 0.f);
	m_vMonsteFront = _vec3(0.f, 0.f, 0.f);
	m_vMonsterLeft = _vec3(0.f, 0.f, 0.f);
	m_vMonsterback = _vec3(0.f, 0.f, 0.f);
	m_vPlayerOriginPos = _vec3(0.f, 0.f, 0.f);
	m_fFront = 0.f;
	m_fBack = 0.f;
	m_fLeft = 0.f;
	m_fRight = 1.f;
	//m_iMonsterIndex
	m_pDynamicTransCom->Set_Scale(&_vec3(3.f, 3.f, 3.f));
	m_pInfoCom->Ready_CharacterInfo(5, 10, 0.7f);
	m_iPreHp = m_pInfoCom->Get_Hp();
	m_fAttackDelay = 3.f;

	m_vPlayerOriginPos = { 0.f,0.f,0.f };


	m_fWaitingTime = 0.35f;

	m_pDynamicTransCom->Update_Component(1.f);
	return S_OK; 
}

_int CFinalBoss::Update_Object(const _float & fTimeDelta)
{
	// 맨위에있어야됌 리턴되면 안됌
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);
	// 맨위에있어야됌 리턴되면 안됌

	m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y * 0.5f);
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	if (Distance_Over())
	{
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}

	if (Dead_Judge(fTimeDelta))
	{
		return 0;
	}
	if (false == m_bCamouflage && false == m_bMorphFinish)
	{
		Camouflage_Cancle(fTimeDelta);
	}

	AttackJudge(fTimeDelta);

	BattleLoop(fTimeDelta);

	m_pDynamicTransCom->Update_Component(1.f);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CFinalBoss::LateUpdate_Object(void)
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

void CFinalBoss::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	Set_Light_Obj();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->LightEnable(0, FALSE);
	m_pGraphicDev->LightEnable(1, FALSE);
	m_pGraphicDev->LightEnable(2, FALSE);
	m_pGraphicDev->LightEnable(3, TRUE);
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	switch (m_bState)
	{
	case Camouflage_Walk:
		m_pCamouTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
		break;

	case Camouflage_Shoot:
		m_pCamouAttackTextureCom->Set_Texture(m_pCamouAttackAnimationCom->m_iMotion);
		break;

	case Morph:
		m_pMorphTextureCom->Set_Texture(m_pMorphAnimationCom->m_iMotion);
		break;

	case Thingy_Walk:
		m_pThingyTextureCom->Set_Texture(m_pThingy_AnimatiionCom->m_iMotion);
		break;

	case Attack_A:
		m_pThingy_OnekiokTextureCom->Set_Texture(m_pThingy_Onekiok_AnimationCom->m_iMotion);
		break;

	case Attack_B:
		m_pThingy_AttackBTextureCom->Set_Texture(m_pThingy_AttackB_AnimationCom->m_iMotion);
		break;

	case Attack_C:
		if ( false == m_bTentacleAnimation)
		{
			m_pThingy_AttackBTextureCom->Set_Texture(m_pThingy_AttackB_AnimationCom->m_iMotion);
		}
		else
		{
			m_pThingy_AttackATextureCom->Set_Texture(m_pThingy_AttackA_AnimationCom->m_iMotion);
		}
		break;

	case Attack_D:
		m_pThingy_AttackCTextureCom->Set_Texture(m_pThingy_AttackC_AnimationCom->m_iMotion);
		break;

	case Thingy_Death:
		m_pDeadTextureCom->Set_Texture(m_pDeadAnimationCom->m_iMotion);
		break;
	}
	SetUp_Material();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->LightEnable(0, TRUE);
	m_pGraphicDev->LightEnable(1, TRUE);
	m_pGraphicDev->LightEnable(2, TRUE);
	m_pGraphicDev->LightEnable(3, FALSE);
}

void CFinalBoss::Collision_Event()
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
		fMtoPDistance < MAX_CROSSROAD + g_fRange  &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(1);
		cout << "Spider " << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);

		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_SPIDER, 0, 7, 0.2f);

	}
}

bool CFinalBoss::Dead_Judge(const _float & fTimeDelta)
{
	if (0 >= m_pInfoCom->Get_Hp())
	{
		if (m_bCamouflage)
		{
			m_bCamouflage = false;
			m_pInfoCom->Add_Hp(4);
		}
		else
		{
			if (false == m_bDead)
			{
				_int Hitsound = rand() % 3;
				m_bState = Thingy_Death;
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Cthulhu_death_01.wav", SOUND_MONSTER, g_fSound);
				m_bDead = true;
			}
		}
		
	}
	if (m_bDead)
	{
		if (m_pDeadAnimationCom->m_iMotion<m_pDeadAnimationCom->m_iMaxMotion)
			m_pDeadAnimationCom->Move_Animation(fTimeDelta);

		if (m_pDeadAnimationCom->m_iMaxMotion == m_pDeadAnimationCom->m_iMotion)
		{
			if (false == m_bShootFlare)
			{
				CScene* pScene = ::Get_Scene();
				CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
				CGameObject* pFlare = nullptr;
				pFlare = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS]);
				
				pMyLayer->Add_EffectList(pFlare);
				pFlare = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS] + _vec3(3.f, 0.f, 3.f));

				pMyLayer->Add_EffectList(pFlare);

				pFlare = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS] + _vec3(-7.f, 0.f, -3.f));

				pMyLayer->Add_EffectList(pFlare);

				pFlare = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS] + _vec3(-6.f, 0.f, 3.f));

				pMyLayer->Add_EffectList(pFlare);
				pFlare = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS] + _vec3(1.5f, 0.f, -6.5f));

				pMyLayer->Add_EffectList(pFlare);
				// 몇개 더
				m_bShootFlare = true;
			}
		}
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);
		return true;
	}
	else
	{
		return false;
	}
}

void CFinalBoss::Camouflage_Attack(const _float & fTimeDelta)
{

	m_pCamouAttackAnimationCom->Move_Animation(fTimeDelta);
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
	m_bState = Camouflage_Shoot;
	m_pCamouAttackAnimationCom->Move_Animation(fTimeDelta);

	if (5 == m_pCamouAttackAnimationCom->m_iMotion && false == m_bShotBullet)
	{
		CGameObject* pBullet;
		pBullet = CHarpoonBullet::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS]);
		pMyLayer->Add_EffectList(pBullet);
		m_bShotBullet = true;

		::StopSound(SOUND_EFFECT2);
		::PlaySoundW(L"LaserGun.wav", SOUND_EFFECT2, g_fSound);
	}
	if (m_pCamouAttackAnimationCom->m_iMaxMotion == m_pCamouAttackAnimationCom->m_iMotion)
	{
		m_bAttack = false;
		m_bState = Camouflage_Walk;

		m_bShotBullet = false;
	}
}

void CFinalBoss::Camouflage_Cancle(const _float & fTimeDelta)
{
	m_pDynamicTransCom->Set_Scale(&_vec3(5.05f, 5.05f, 5.05f));
	m_bState = Morph;
	m_pMorphAnimationCom->Move_Animation(fTimeDelta);
	if (false == m_bMorphSound)
	{
		::StopSound(SOUND_MONSTER);
		::PlaySoundW(L"Cthulhu_pain_01.wav", SOUND_MONSTER, g_fSound);
	}
	if (m_pMorphAnimationCom->m_iMaxMotion == m_pMorphAnimationCom->m_iMotion)
	{
		m_bMorphFinish = true;
		m_bState = Thingy_Walk;
	}
}

void CFinalBoss::AttackJudge(const _float & fTimeDelta)
{
	if (m_bAttack == false)
	{
		m_fAttackDelayTime += fTimeDelta;
		
		if (m_fAttackDelay <= m_fAttackDelayTime)
		{
			m_bAttack = true;
			m_iAttackPattern = rand() % 5; //rand
			
			m_fAttackDelayTime = 0.f;
		}
	}
	if (true == m_bCamouflage)
	{
		if (m_pCamouAttackAnimationCom->m_iMotion<m_pCamouAttackAnimationCom->m_iMaxMotion
			&& m_pCamouAttackAnimationCom->m_iMotion>m_pCamouAttackAnimationCom->m_iMinMotion)
		{
			m_bAttacking = true;
		}
		else
		{
			m_bAttacking = false;
		}
	}

}

void CFinalBoss::Attack(const _float & fTimeDelta)
{
	if (false == m_bAttackSound)
	{
		::StopSound(SOUND_MONSTER2);
		::PlaySoundW(L"Cthulhu_attack_01.wav", SOUND_MONSTER2, g_fSound);
		m_bAttackSound = true;
	}
	switch (m_iAttackPattern)
	{
	case 0:
		m_bState = Attack_A;
		AttackPettern1(fTimeDelta);
		break;

	case 1:
		m_bState = Attack_B;
		AttackPettern2(fTimeDelta);
		break;

	case 2:
		m_bState = Attack_C;
		AttackPettern3(fTimeDelta);
		break;

	case 3:
		m_bState = Attack_D;
		AttackPettern4(fTimeDelta);
		break;
	}
}

void CFinalBoss::BattleLoop(const _float & fTimeDelta)
{
	
	if (m_bCamouflage)
	{
		if (fMtoPDistance > 10.f && m_bAttacking == false)
		{
			m_bState = Camouflage_Walk;
			m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);

			m_pAnimationCom->Move_Animation(fTimeDelta);
		}
		else
		{
			if (m_bAttack)
			{
				Camouflage_Attack(fTimeDelta);
			}
			else
			{
				m_pAnimationCom->m_iMotion = 0;
				m_bState = Camouflage_Walk;
			}
		}
	}
	else if(true == m_bMorphFinish)
	{
		if (fMtoPDistance > 10.f && Thingy_Walk == m_bState)
		{
			
			m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);

			m_pThingy_AnimatiionCom->Move_Animation(fTimeDelta);
		}
		else
		{
			if (m_bAttack)
			{
				Attack(fTimeDelta);
			}
			else
			{
				m_pThingy_AnimatiionCom->m_iMotion = 0;
				m_bState = Thingy_Walk;
			}
		}
	}
}
void	CFinalBoss::AttackPettern1(const _float& fTimeDelta)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
	if (6 != m_pThingy_Onekiok_AnimationCom->m_iMotion || m_bOnekiokGo)
	{
		m_pThingy_Onekiok_AnimationCom->Move_Animation(fTimeDelta);
	}
	if (6 == m_pThingy_Onekiok_AnimationCom->m_iMotion && false == m_bCreatedOnekiok)
	{
		CGameObject* pBullet = nullptr;
		pBullet = CFinalBossBullet::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS] + _vec3(0.f, 1.f, 0.f), this);
			
		m_bCreatedOnekiok = true;
		pMyLayer->Add_EffectList(pBullet);

	}
	if (m_pThingy_Onekiok_AnimationCom->m_iMaxMotion == m_pThingy_Onekiok_AnimationCom->m_iMotion)
	{
		m_bAttack = false;
		m_bOnekiokGo = false;
		m_pThingy_Onekiok_AnimationCom->m_iMotion = 0;
		m_bCreatedOnekiok = false;
		m_bState = Thingy_Walk;
		m_bAttackSound = false;
	}

}

void	CFinalBoss::AttackPettern2(const _float& fTimeDelta)
{

	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	m_pThingy_AttackB_AnimationCom->Move_Animation(fTimeDelta);
	if (11 == m_pThingy_AttackB_AnimationCom->m_iMotion)
	{
		if (false == m_bCreatedTapeWorm)
		{
			CGameObject* pTapeWorm = nullptr;
			pTapeWorm = CTapeWorm::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS].x, m_pDynamicTransCom->m_vInfo[INFO_POS].z);
			pMyLayer->Add_GhulList(pTapeWorm);
			m_bCreatedTapeWorm = true;
		}
	}
	if (m_pThingy_AttackB_AnimationCom->m_iMaxMotion == m_pThingy_AttackB_AnimationCom->m_iMotion)
	{
		m_bCreatedTapeWorm = false;
		m_bAttack = false;
		m_bState = Thingy_Walk;
		m_bAttackSound = false;
		m_pThingy_AttackB_AnimationCom->m_iMotion = 0;
	}
}
void CFinalBoss::AttackPettern3(const _float & fTimeDelta)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
	if (false == m_bTentacleAnimation)
	{
		m_pThingy_AttackB_AnimationCom->Move_Animation(fTimeDelta);
		if (11 == m_pThingy_AttackB_AnimationCom->m_iMotion)
		{
			//촉수 생성
			if (false == m_bCreateTentacle)
			{
				CGameObject* pTentacle = nullptr;
				pTentacle = CTentacle::Create(m_pGraphicDev, this, 0, m_vPlayerPos.x, m_vPlayerPos.z);
				pMyLayer->Add_EffectList(pTentacle);
				m_bCreateTentacle = true;
			}
		}
		if (m_pThingy_AttackB_AnimationCom->m_iMaxMotion == m_pThingy_AttackB_AnimationCom->m_iMotion)
		{
			m_bTentacleAnimation = true;
			m_pThingy_AttackB_AnimationCom->m_iMotion = 0;
		}
	}
	if (m_bCreateTentacle)
	{
		//if (m_iPreHp > m_pInfoCom->Get_Hp())
		//{
		//	m_iHitNum++;
		//	if (2 < m_iHitNum)
		//		m_bPettern3Finish = true;
		//}
	}
	if (true == m_bTentacleAnimation)
	{
		if (1.5f > fMtoPDistance && 7==m_pThingy_AttackA_AnimationCom->m_iMotion)
		{
			CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
			
			pPlayerInfo->Receive_Damage(10);
			m_bPettern3Finish = true;
		}
		if( 7>m_pThingy_AttackA_AnimationCom->m_iMotion || m_bPettern3Finish)
		m_pThingy_AttackA_AnimationCom->Move_Animation(fTimeDelta);

		if (m_pThingy_AttackA_AnimationCom->m_iMaxMotion == m_pThingy_AttackA_AnimationCom->m_iMotion)
		{

			m_bAttack = false;
			m_bTentacleAnimation = false;
			m_bCreateTentacle = false;
			m_bPettern3Finish = false;
			m_iHitNum = 0;
			m_pThingy_AttackA_AnimationCom->m_iMotion = 0;
			m_bState = Thingy_Walk;
			m_bAttackSound = false;
		}
	}

	
}
void CFinalBoss::AttackPettern4(const _float & fTimeDelta)
{
	if (false == m_bSavePosition)
	{
		_vec3 ShakerPos, vDir, vMonsterRight;
		m_pDynamicTransCom->Get_Info(INFO_POS, &ShakerPos);
		vDir = m_vPlayerOriginPos - ShakerPos;
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Cross(&vMonsterRight, &_vec3(0.f, 1.f, 0.f), &vDir);
		m_vMonsteFront = vDir;
		m_vMonsterRight = vMonsterRight;
		m_vMonsterback = -vDir;
		m_vMonsterLeft = -vMonsterRight;
		m_bSavePosition = true;
	}
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
	m_pThingy_AttackC_AnimationCom->Move_Animation(fTimeDelta);

	if (4 == m_pThingy_AttackC_AnimationCom->m_iMotion && false== m_bCreateSpike)
	{
		
		//촉ㄹ수촉촉수


		CThingySpike* pTentacle = nullptr;
		::StopSound(SOUND_MONSTER2);
		::PlaySoundW(L"Qoong.wav", SOUND_MONSTER2, g_fSound);
		if (m_bPettern4LR)
		{
			for (int i = 1; i < 20; ++i)
			{
				_vec3 ShakerPos;
				m_pDynamicTransCom->Get_Info(INFO_POS, &ShakerPos);
				_vec3 vTempDir = _vec3(0.f, 0.f, 0.f);
				vTempDir = i * (m_vMonsteFront * m_fFront + m_vMonsterRight * m_fRight + m_vMonsterback * m_fBack + m_vMonsterLeft * m_fLeft);

				
				pTentacle = CThingySpike::Create(m_pGraphicDev, m_fWaitingTime * i, ShakerPos.x + vTempDir.x, ShakerPos.z + vTempDir.z);
				pMyLayer->Add_EffectList(pTentacle);

				pTentacle = CThingySpike::Create(m_pGraphicDev, m_fWaitingTime * i, ShakerPos.x + -vTempDir.x, ShakerPos.z + -vTempDir.z);
				pMyLayer->Add_EffectList(pTentacle);

			
			}
		}
		else
		{
			for (int i = 1; i < 20; ++i)
			{
				_vec3 ShakerPos;
				m_pDynamicTransCom->Get_Info(INFO_POS, &ShakerPos);
				_vec3 vTempDir = _vec3(0.f, 0.f, 0.f);
				vTempDir = i * (m_vMonsteFront * m_fFront + m_vMonsterRight * m_fRight + m_vMonsterback * m_fBack + m_vMonsterLeft * m_fLeft);


				pTentacle = CThingySpike::Create(m_pGraphicDev, m_fWaitingTime * i, ShakerPos.x + vTempDir.z, ShakerPos.z + vTempDir.x);
				pMyLayer->Add_EffectList(pTentacle);

				pTentacle = CThingySpike::Create(m_pGraphicDev, m_fWaitingTime * i, ShakerPos.x + -vTempDir.z, ShakerPos.z + -vTempDir.x);
				pMyLayer->Add_EffectList(pTentacle);


			}
		}
		//m_fRight 를 front로
		switch (m_iOneDirrection)
		{
		case 0:
			m_fRight -= 0.1f;
			m_fFront += 0.1f;
			break;
		case 1:
			m_fFront -= 0.1f;
			m_fLeft += 0.1f;
			break;
		case 2:
			m_fLeft -= 0.1f;
			m_fBack += 0.1f;
			break;
		case 3:
			m_fBack -= 0.1f;
			m_fRight += 0.1f;
			break;
		default:
			m_iOneDirrection = 0;
			break;
		}
		if (0.1f > m_fRight && 0.9f < m_fFront)
		{
			m_iOneDirrection = 1;
		}
		else if (0.1f > m_fFront && 0.9f < m_fLeft)
		{
			m_iOneDirrection = 2;
		}
		else if (0.1f > m_fLeft && 0.9f < m_fBack)
		{
			m_iOneDirrection = 3;
		}
		else if (0.1f > m_fBack && 0.9f < m_fRight)
		{
			m_iOneDirrection = 0;
		}

		//플레이어 Dir도 곱해서 더해주면 될듯
		//for (int i = 1; i < 20; ++i)
		//{
		//	pTentacle = CThingySpike::Create(m_pGraphicDev, m_fWaitingTime * i, ShakerPos.x + vMonsterRight.x * i * 0.5f, ShakerPos.z + vMonsterRight.z * i * 0.5f);
		//	pMyLayer->Add_EffectList(pTentacle);

		//	pTentacle = CThingySpike::Create(m_pGraphicDev, m_fWaitingTime * i, ShakerPos.x - vMonsterRight.x * i * 0.5f, ShakerPos.z - vMonsterRight.z * i * 0.5f);
		//	pMyLayer->Add_EffectList(pTentacle);
		//	//m_Spikelist.push_back(pSpike);
		//}
		m_bCreateSpike = true;
	}
	if (9 == m_pThingy_AttackC_AnimationCom->m_iMotion && m_iPettern4RepeatNum <20)
	{
		m_iPettern4RepeatNum++;
		m_pThingy_AttackC_AnimationCom->m_iMotion = 3;
		m_bCreateSpike = false;
	}
	if (m_pThingy_AttackC_AnimationCom->m_iMaxMotion == m_pThingy_AttackC_AnimationCom->m_iMotion)
	{
		m_bAttack = false;
		m_bState = Thingy_Walk;
		m_bCreateSpike = false;
		m_iPettern4RepeatNum = 0;
		m_vPlayerOriginPos = _vec3(0.f, 0.f, 0.f);
		m_bSavePosition = false;
		m_bAttackSound = false;

		m_fRight = 1.f;
		m_fFront = 0.f;
		m_fLeft = 0.f;
		m_fBack = 0.f;
		m_iOneDirrection = 0;
		if (false == m_bPettern4LR)
		{
			m_bPettern4LR = true;
		}
		else
			m_bPettern4LR = false;
	}
}
HRESULT CFinalBoss::SetUp_Material(void)
{

	if (Get_Distance() < 50.f)
	{
		_float fDistance = ((1.f - Get_Distance() / 50.f)) * 2;
		max(fDistance, 0.1f);
		D3DMATERIAL9		tMtrl;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

		tMtrl.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Emissive = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Power = 0.f;

		m_pGraphicDev->SetMaterial(&tMtrl);

	}
	else
	{
		D3DMATERIAL9		tMtrl;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

		tMtrl.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Emissive = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		tMtrl.Power = 0.f;

		m_pGraphicDev->SetMaterial(&tMtrl);
	}


	return S_OK;
}
void CFinalBoss::Set_Light_Obj()
{
	if (Get_Distance() >= 51.f)
	{
		D3DLIGHT9		tLightInfo4;
		ZeroMemory(&tLightInfo4, sizeof(D3DLIGHT9));
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo4, 3), );
		return;
	}

	if (Get_Distance() < 50.f)
	{
		_float fDistance = ((1.f - Get_Distance() / 50.f))*2.f;;
		max(fDistance, 0.1f);
		D3DLIGHT9		tLightInfo4;
		ZeroMemory(&tLightInfo4, sizeof(D3DLIGHT9));
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		tLightInfo4.Type = D3DLIGHT_SPOT;
		tLightInfo4.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);

		tLightInfo4.Position = vPos;
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo4, 3), );

	}
}
CFinalBoss * CFinalBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy)
{
	CFinalBoss*	pInstance = new CFinalBoss(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalBoss::Free(void)
{
	CMonsterBase::Free();
}
