#include "stdafx.h"
#include "..\Header\FinalBoss.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "HitEffect.h"
#include "Player.h"
#include "Gun_Screen.h"


CFinalBoss::CFinalBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CFinalBoss::~CFinalBoss()
{
}

HRESULT CFinalBoss::Ready_Object(float Posx, float Posy, float Size)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pCamouTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Harpoonguy_Texture", m_mapComponent, ID_STATIC);
	m_pCamouAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Harpoonguy_Attack_Texture", m_mapComponent, ID_STATIC);
	
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

	AttackJudge(fTimeDelta);

	BattleLoop(fTimeDelta);

	return 0;
}

void CFinalBoss::LateUpdate_Object(void)
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
				::PlaySoundW(L".wav", SOUND_MONSTER, 0.4f);
				break;

			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L".wav", SOUND_MONSTER, 0.4f);
				break;

			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L".wav", SOUND_MONSTER, 0.4f);
				break;
			}
		}
	}
}

void CFinalBoss::Render_Obejct(void)
{

}

void CFinalBoss::Collision_Event()
{
}

bool CFinalBoss::Dead_Judge(const _float & fTimeDelta)
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
				::PlaySoundW(L".wav", SOUND_MONSTER, 0.4f);
				break;
			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L".wav", SOUND_MONSTER, 0.4f);
				break;
			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L".wav", SOUND_MONSTER, 0.4f);
				break;
			}
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

void CFinalBoss::Camouflage_Attack(const _float & fTimeDelta)
{
}

void CFinalBoss::Camouflage_Cancle(const _float & fTimeDelta)
{
}

void CFinalBoss::AttackJudge(const _float & fTimeDelta)
{
	if (m_bAttack == false)
	{
		m_fAttackDelayTime += fTimeDelta;
		
		if (m_fAttackDelay <= m_fAttackDelayTime)
		{
			m_bAttack = true;
			m_iAttackPattern = rand() % 2; //rand
			m_fAttackDelayTime = 0.f;
		}
	}
}

void CFinalBoss::Attack(const _float & fTimeDelta)
{
	switch (m_iAttackPattern)
	{
	case 0:

		break;

	case 1:

		break;

	case 2:

		break;
	}
}

void CFinalBoss::BattleLoop(const _float & fTimeDelta)
{
	if (m_bAttack)
	{
		if (m_bCamouflage)
		{
			Camouflage_Attack(fTimeDelta);
		}
		else
		{
			Attack(fTimeDelta);
		}
	}
}

CFinalBoss * CFinalBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy, float Size)
{
	CFinalBoss*	pInstance = new CFinalBoss(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, Size)))
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
