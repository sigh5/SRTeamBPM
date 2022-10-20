#include "stdafx.h"
#include "..\Header\TapeWorm.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"
#include "HitEffect.h"
#include "Gun_Screen.h"


CTapeWorm::CTapeWorm(LPDIRECT3DDEVICE9 pGraphicDev)
		:CMonsterBase(pGraphicDev)
{
}


CTapeWorm::~CTapeWorm()
{
}

HRESULT CTapeWorm::Ready_Object(float Posx, float Posz)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_TapeWorm_Texture", m_mapComponent, ID_STATIC);
	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_TapeWorm_death_Texture", m_mapComponent, ID_STATIC);

	m_pAnimationCom->Ready_Animation(27, 0, 0.15f);

	m_pDeadAnimationCom->Ready_Animation(5, 0, 0.3f);

	if (Posx == 0 && Posz == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((_float)Posx, m_pDynamicTransCom->m_vScale.y * 0.5f, (_float)Posz);
	}
	Save_OriginPos();

	m_pInfoCom->Ready_CharacterInfo(2, 10, 2.5f);

	m_fAttackDelay = 0.7f;

	m_bAttack = false;

	_vec3 vPos, vScale;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pColliderCom->Set_vCenter(&vPos, &vScale);

	return S_OK;
}

_int CTapeWorm::Update_Object(const _float & fTimeDelta)
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

	NoHit_Loop(fTimeDelta);

	m_pDynamicTransCom->Update_Component(fTimeDelta);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	if(m_bAttack)
	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CTapeWorm::LateUpdate_Object(void)
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

void CTapeWorm::Render_Obejct(void)
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
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
	}
	else
	{
		m_pDeadTextureCom->Set_Texture(m_pDeadAnimationCom->m_iMotion);
	}
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CTapeWorm::Collision_Event()
{
	if (2 < m_pAnimationCom->m_iMotion && 21 > m_pAnimationCom->m_iMotion)
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
}

void CTapeWorm::Excution_Event()
{
}

bool CTapeWorm::Dead_Judge(const _float & fTimeDelta)
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

void CTapeWorm::NoHit_Loop(const _float & fTimeDelta)
{

	if (m_bAttack)
	{
		Attack(fTimeDelta);
	}
	else
	{
		m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);
	}
}


void CTapeWorm::AttackJudge(const _float & fTimeDelta)
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
}

void CTapeWorm::Attack(const _float & fTimeDelta)
{
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));

	m_pAnimationCom->Move_Animation(fTimeDelta);
	if (2 < m_pAnimationCom->m_iMotion && 14 > m_pAnimationCom->m_iMotion)
	{
		if (fMtoPDistance < 1.5f && false == m_bHitPlayer)
		{
			pPlayerInfo->Receive_Damage(10);
			m_bHitPlayer = true;
		}
	}
	if (m_pAnimationCom->m_iMaxMotion == m_pAnimationCom->m_iMotion)
	{
		m_bAttack = false;
		m_bHitPlayer = false;
	}
}

CTapeWorm * CTapeWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posz)
{
	CTapeWorm*	pInstance = new CTapeWorm(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posz)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTapeWorm::Free(void)
{
	CMonsterBase::Free();
}
