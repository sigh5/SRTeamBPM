#include "stdafx.h"
#include "..\Header\SphinxFlyHead.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"
#include "Gun_Screen.h"
#include "Flare.h"
#include "MonsterHpBar.h"
#include "Stage.h"

CSphinxFlyHead::CSphinxFlyHead(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CSphinxFlyHead::~CSphinxFlyHead()
{
}

HRESULT CSphinxFlyHead::Ready_Object(float Posx, float Posy, float Size)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_FlyHead_Texture", m_mapComponent, ID_STATIC);
	m_pActivatedTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_Activated_Texture", m_mapComponent, ID_STATIC);
	m_pLRAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_lrattack_Texture", m_mapComponent, ID_STATIC);
	m_pBodyAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_bodyattack_Texture", m_mapComponent, ID_STATIC);
	m_pDeadTexture = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_dead_Texture", m_mapComponent, ID_STATIC);

	m_pHeadActivatedAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pHeadActivatedAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Head_Active_AnimationCom", m_pHeadActivatedAnimationCom });

	m_pLRAttackAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pLRAttackAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LRAttack_AnimationCom", m_pLRAttackAnimationCom });

	m_pBodyAttackAnimation = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pBodyAttackAnimation, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_bodyAttack_AnimationCom", m_pBodyAttackAnimation });

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom->Ready_Animation(7, 0, 100);

	m_pHeadActivatedAnimationCom->Ready_Animation(5, 0, 0.5f);
	m_pLRAttackAnimationCom->Ready_Animation(4, 0, 0.5f);
	m_pBodyAttackAnimation->Ready_Animation(8, 0, 0.4f);
	m_pDeadAnimationCom->Ready_Animation(23, 0, 0.3f);

	m_pInfoCom->Ready_CharacterInfo(150, 10, 2.f);

	for (int i = 0; i < 4; ++i)
	{
		m_bArrFalldown[i] = false;
	}

	m_iAttackPattern = 1;
	m_fAttackDelay = 0.4f;

	m_fTackleSpeed = 3.5f;
	m_fTackleAttenuationTime = 0.01f;
	m_fTackleAttenuationTimeCount = 0;
	m_fTackleStopper = 0.05f;
	m_fOriginTackleStopper = m_fTackleStopper;


	m_iMonsterIndex = MONSTER_FLY_HEAD;
	m_fRearrangementDelay = 1.f;

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos(Posx, Size*0.5f, Posy);
	}
	m_pDynamicTransCom->Set_Scale(&_vec3{ Size, Size, 1.f });
	m_fLimitY = m_pDynamicTransCom->m_vScale.y * 0.2f;



	Save_OriginPos();
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	_vec3 MonsterPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &MonsterPos);
	_vec3 dir = m_vPlayerPos - MonsterPos;
	//D3DXVec3Normalize(&dir, &dir);
	m_pColliderCom->Set_HitRadiuos(0.f);
	m_pDynamicTransCom->Set_CountMovePos(&(-dir * 0.2f));
	m_pDynamicTransCom->Update_Component(1.f);


	return S_OK;
}

_int CSphinxFlyHead::Update_Object(const _float & fTimeDelta)
{

	// 맨위에있어야됌 리턴되면 안됌
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);
	// 맨위에있어야됌 리턴되면 안됌
	Add_HpBar();
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	m_fVolume = (100 - fMtoPDistance) * 0.01f * (g_fSound * 2.f);
	if (Distance_Over())
	{
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}
	if (false == m_bBattle)
		HeadActive(fTimeDelta);
	if (Dead_Judge(fTimeDelta))
	{
		return 0;
	}

	m_fTimeDelta = fTimeDelta;

	AttackJudge(fTimeDelta);

	if (m_bBattle)
		BattleLoop(fTimeDelta);

	if (m_fLimitY > m_pDynamicTransCom->m_vInfo[INFO_POS].y)
	{
		m_pDynamicTransCom->m_vInfo[INFO_POS].y = m_fLimitY;
	}
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	
	NULL_CHECK_RETURN(pScene, RETURN_ERR);

	if (Get_Distance() < 70.f)
		static_cast<CStage*>(pScene)->Set_SphinxHeadBGM(false);

	else
		static_cast<CStage*>(pScene)->Set_SphinxHeadBGM(true);

	//pMyLayer->Add_vecColliderMonster(static_cast<CMonsterBase*>(this));
	return 0;
}

void CSphinxFlyHead::LateUpdate_Object(void)
{
	::SetChannelVolume(SOUND_EFFECT2, m_fVolume);

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

	Engine::CMonsterBase::LateUpdate_Object();
}

void CSphinxFlyHead::Render_Obejct(void)
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
		if (false == m_bBattle)
		{
			m_pActivatedTextureCom->Set_Texture(m_pHeadActivatedAnimationCom->m_iMotion);
		}
		else
		{
			switch (m_iAttackPattern)
			{
			case 0:    //LR
				m_pLRAttackTextureCom->Set_Texture(m_pLRAttackAnimationCom->m_iMotion);
				break;

			case 1:		//BodyAttack
				if (m_bRenderBodyAttack)
					m_pBodyAttackTextureCom->Set_Texture(m_pBodyAttackAnimation->m_iMotion);
				else
					m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
				//m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
				break;
			}
		}
	}
	else
	{
		m_pDeadTexture->Set_Texture(m_pDeadAnimationCom->m_iMotion);
	}
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CSphinxFlyHead::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));


	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() &&
		fMtoPDistance < MAX_CROSSROAD + g_fRange &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(pPlayerInfo->Get_AttackPower());
		cout << "Obelisk " << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);
	}
}

void		CSphinxFlyHead::HeadActive(const _float& fTimeDelta)
{
	m_pHeadActivatedAnimationCom->Move_Animation(fTimeDelta);
	if (m_pHeadActivatedAnimationCom->m_iMaxMotion <= m_pHeadActivatedAnimationCom->m_iMotion)
	{
		m_bBattle = true;
	}
}

void CSphinxFlyHead::AttackJudge(const _float & fTimeDelta)
{
	if (m_bAttack == false)
	{
		m_fAttackDelayTime += fTimeDelta;

		if (m_pAnimationCom->m_iMotion != 0)
		{
			Rearrangement(fTimeDelta);
		}
		else
		{
			if (m_fAttackDelay <= m_fAttackDelayTime)
			{
				m_bAttack = true;
				m_iAttackPattern = rand() % 2; //rand
				m_fAttackDelayTime = 0.f;
			}
		}
	}
}

void CSphinxFlyHead::BattleLoop(const _float & fTimeDelta)
{
	if (m_bAttack)
	{
		Attack(fTimeDelta);
	}

}

void	CSphinxFlyHead::Attack(const _float& fTimeDelta)
{
	switch (m_iAttackPattern)
	{
	case 0:
		AttackLeftRight(fTimeDelta);
		break;

	case 1:
		BodyAttack(fTimeDelta);
		break;

	}

}

void		CSphinxFlyHead::AttackLeftRight(const _float& fTimeDelta)
{
	m_pLRAttackAnimationCom->m_fMotionChangeCounter += fTimeDelta;
	if (4 == m_pLRAttackAnimationCom->m_iMotion)
	{
		if (false == m_bLRAttackSound)
		{
			::StopSound(SOUND_EFFECT2);
			::PlaySoundW(L"executor_spell_sound.wav", SOUND_EFFECT2, m_fVolume);
			m_bLRAttackSound = true;
		}
		LeftAttack(fTimeDelta);
	
	}
	if (2 == m_pLRAttackAnimationCom->m_iMotion)
	{
		if (false == m_bLRAttackSound)
		{
			::StopSound(SOUND_EFFECT2);
			::PlaySoundW(L"executor_spell_sound.wav", SOUND_EFFECT2, m_fVolume);
			m_bLRAttackSound = true;
		}
		RightAttack(fTimeDelta);
	
	}
	if (3 == m_pLRAttackAnimationCom->m_iMotion || 1 == m_pLRAttackAnimationCom->m_iMotion)
	{
		if (false == m_bGet_PlayerPos_LR&&2.9f < m_pLRAttackAnimationCom->m_fMotionChangeCounter)
		{
			Save_PlayerPos_forLR(fTimeDelta);
		}
		if (3.f < m_pLRAttackAnimationCom->m_fMotionChangeCounter)
		{
			++m_pLRAttackAnimationCom->m_iMotion;
			m_pLRAttackAnimationCom->m_fMotionChangeCounter = 0.f;
		}
	}
	if (0 == m_pLRAttackAnimationCom->m_iMotion)
	{
		LeftRightJudge(fTimeDelta);
		if (false == m_bLRChargeSound)
		{
			::StopSound(SOUND_EFFECT2);
			::PlaySoundW(L"curse_spell_loop_sound.wav", SOUND_EFFECT2, m_fVolume);
			m_bLRChargeSound = true;
		}
	}



}

void CSphinxFlyHead::LeftRightJudge(const _float & fTimeDelta)
{
	if (false == m_bSelectedLeftRight)
	{
		m_iLRJudge = rand() % 2;
		m_bSelectedLeftRight = true;
	}

	if (m_pLRAttackAnimationCom->m_fIntervalMotion < m_pLRAttackAnimationCom->m_fMotionChangeCounter)
	{
		m_pLRAttackAnimationCom->m_fMotionChangeCounter = 0.f;
		switch (m_iLRJudge)
		{
		case 0:
			m_pLRAttackAnimationCom->m_iMotion = 1;
			break;

		case 1:
			m_pLRAttackAnimationCom->m_iMotion = 3;
			break;
		}
	}

}

void CSphinxFlyHead::LeftAttack(const _float & fTimeDelta)
{
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vCurPlayerPosdir;
	vCurPlayerPosdir = m_vPlayerPos - m_pDynamicTransCom->m_vInfo[INFO_POS];

	float fValue = D3DXVec3Dot(&m_vPrePlayerPosdir, &vCurPlayerPosdir);
	if (0 <= fValue)
	{
		pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
	}
	if (m_pLRAttackAnimationCom->m_fIntervalMotion < m_pLRAttackAnimationCom->m_fMotionChangeCounter)
	{
		m_bAttack = false;
		m_pLRAttackAnimationCom->m_iMotion = 0;
		m_pLRAttackAnimationCom->m_fMotionChangeCounter = 0.f;
		m_bGet_PlayerPos_LR = false;
		m_bSelectedLeftRight = false;
		m_iLRJudge = 0;
		m_bLRChargeSound = false;
		m_bLRAttackSound = false;
	}
}

void CSphinxFlyHead::RightAttack(const _float & fTimeDelta)
{
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vCurPlayerPosdir;
	vCurPlayerPosdir = m_vPlayerPos - m_pDynamicTransCom->m_vInfo[INFO_POS];

	float fValue = D3DXVec3Dot(&m_vPrePlayerPosdir, &vCurPlayerPosdir);
	if (0 >= fValue)
	{
		pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
	}
	if (m_pLRAttackAnimationCom->m_fIntervalMotion < m_pLRAttackAnimationCom->m_fMotionChangeCounter)
	{
		m_bAttack = false;
		m_pLRAttackAnimationCom->m_iMotion = 0;
		m_pLRAttackAnimationCom->m_fMotionChangeCounter = 0.f;
		m_bGet_PlayerPos_LR = false;
		m_bSelectedLeftRight = false;
		m_iLRJudge = 0;
		m_bLRChargeSound = false;
		m_bLRAttackSound = false;
	}
}

void	CSphinxFlyHead::Save_PlayerPos_forLR(const _float& fTimeDelta)
{
	m_vPrePlayerPosdir = m_vPlayerPos - m_pDynamicTransCom->m_vInfo[INFO_POS];
	D3DXVec3Cross(&m_vPrePlayerPosdir, &m_vPrePlayerPosdir, &_vec3{ 0.f, 1.f, 0.f });
	//D3DXVec3Normalize(&m_vPrePlayerPosdir, &m_vPrePlayerPosdir);
	m_bGet_PlayerPos_LR = true;
}

void	CSphinxFlyHead::BodyAttack(const _float& fTimeDelta)
{
	switch (m_iBodyAttackLevel)
	{
	case 0:
		//Ready level
		if (false == m_bChargeSound)
		{
			::StopSound(SOUND_EFFECT2);
			::PlaySoundW(L"Energy_Shield_Looping_1.wav", SOUND_EFFECT2, m_fVolume);
			m_bChargeSound = true;
		}
		m_bRenderBodyAttack = true;
		m_pBodyAttackAnimation->Move_Animation(fTimeDelta);
		if (m_pBodyAttackAnimation->m_iMaxMotion <= m_pBodyAttackAnimation->m_iMotion)
		{
			m_pBodyAttackAnimation->m_iMotion = 0;
			m_iBodyAttackLevel = 1;
			m_bRenderBodyAttack = false;
		}
		break;

	case 1:
		//Set direction
		Save_PlayerPos_forBody(fTimeDelta);
		m_iBodyAttackLevel = 2;
		break;

	case 2:
		Tackle(fTimeDelta);
		if (false == m_bBodyAttackSound)
		{
			_int BodyAttacksound = rand() % 2;
			switch (BodyAttacksound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"satan_transform_2to3.wav", SOUND_MONSTER, m_fVolume);
				break;

			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"satan_transform_3to4.wav", SOUND_MONSTER, m_fVolume);
				break;
			}
			
			m_bBodyAttackSound = true;
		}
		if (m_bAttack == false)
		{
			m_iBodyAttackLevel = 0;
			m_bBodyAttackSound = false;
			m_bChargeSound = false;
		
		}
		break;


	}
}
void		CSphinxFlyHead::Save_PlayerPos_forBody(const _float& fTimeDelta)
{
	m_bGet_PlayerPos_Body = true;

	m_vTackleDir = m_vPlayerPos - m_pDynamicTransCom->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vTackleDir, &m_vTackleDir);
	m_bTackleStart = true;
}
void		CSphinxFlyHead::Tackle(const _float& fTimeDelta)
{

	_vec3 vThunderPos = m_pDynamicTransCom->m_vInfo[INFO_POS];
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vPlayerPos = pPlayerTransformCom->m_vInfo[INFO_POS];
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	float fDistance = sqrtf((powf(vThunderPos.x - vPlayerPos.x, 2) + powf(vThunderPos.y - vPlayerPos.y, 2) + powf(vThunderPos.z - vPlayerPos.z, 2)));

	if (fDistance < 2.f && false == m_bHitPlayer)
	{
		pPlayerInfo->Receive_Damage(10);
		pPlayer->Set_DefenseToHp(true);
		m_bHitPlayer = true;
	}

	m_fTackleAttenuationTimeCount += fTimeDelta;
	Tackle_HeadSpin(fTimeDelta);
	if (m_bAttenuationStart)
	{
		if (m_fTackleAttenuationTime < m_fTackleAttenuationTimeCount)
		{
			m_fTackleStopper *= 1.1f;
			m_fTackleAttenuationTimeCount = 0.f;
		}
	}
	else
	{
		if (2.f < m_fTackleAttenuationTimeCount)
		{
			m_bAttenuationStart = true;
			m_fTackleAttenuationTimeCount = 0.f;
		}
	}
	if (0<m_fTackleSpeed - m_fTackleStopper)
		m_pDynamicTransCom->Move_Pos(&(m_vTackleDir * (m_fTackleSpeed - m_fTackleStopper) * 0.1f));
	else
	{
		m_fTackleStopper = m_fOriginTackleStopper;
		m_bTackleStart = false;
		m_bGet_PlayerPos_Body = false;
		m_bBodyAttackChargeFinish = false;
		m_bAttack = false;
		m_bAttenuationStart = false;
		m_bHitPlayer = false;

	}
	m_pDynamicTransCom->Update_Component(fTimeDelta);
}

void	CSphinxFlyHead::Tackle_HeadSpin(const _float& fTimeDelta)
{
	_vec3 vNowdir, vleft;
	vNowdir = m_vPlayerPos - m_pDynamicTransCom->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vNowdir, &vNowdir);

	D3DXVec3Cross(&vleft, &m_vTackleDir, &_vec3(0.f, 1.f, 0.f));

	float fFrontBack = D3DXVec3Dot(&m_vTackleDir, &vNowdir);

	if (0 < fFrontBack)
	{
		m_bFrontback = true;
	}
	else
	{
		m_bFrontback = false;
	}

	float fLeftRight = D3DXVec3Dot(&vleft, &vNowdir);

	if (0 < fLeftRight)
	{
		m_bLeftRight = true;
	}
	else
	{
		m_bLeftRight = false;
	}

	float fDegree = acosf(fFrontBack);

	if (m_bFrontback)
	{
		if (m_bLeftRight)
		{//14분면
			if (0.7 < fDegree)
			{
				m_pAnimationCom->m_iMotion = 2;
			}
			else if (0.3 < fDegree)
			{
				m_pAnimationCom->m_iMotion = 1;
			}
			else if (0 < fDegree)
			{
				m_pAnimationCom->m_iMotion = 0;
			}

		}
		else
		{//24분면
			if (0.7 < fDegree)
			{
				m_pAnimationCom->m_iMotion = 6;
			}
			else if (0.3 < fDegree)
			{
				m_pAnimationCom->m_iMotion = 7;
			}
			else if (0 < fDegree)
			{
				m_pAnimationCom->m_iMotion = 0;
			}
		}
	}
	else
	{
		float minusdgree = acosf(D3DXVec3Dot(&m_vTackleDir, &-vNowdir));

		if (m_bLeftRight)
		{//34분면
			if (0.8 < minusdgree)
			{
				m_pAnimationCom->m_iMotion = 2;
			}
			else if (0.3 < minusdgree)
			{
				m_pAnimationCom->m_iMotion = 3;
			}
			else if (0 < minusdgree)
			{
				m_pAnimationCom->m_iMotion = 4;
			}

		}
		else
		{//44분면
			if (0.8 < minusdgree)
			{
				m_pAnimationCom->m_iMotion = 6;
			}
			else if (0.3 < minusdgree)
			{
				m_pAnimationCom->m_iMotion = 5;
			}
			else if (0 < minusdgree)
			{
				m_pAnimationCom->m_iMotion = 4;
			}
		}
	}

}

void	CSphinxFlyHead::Rearrangement(const _float& fTimeDelta)
{
	m_fRearrangementDealyCount += fTimeDelta;
	if (m_pDynamicTransCom->m_vScale.y > m_pDynamicTransCom->m_vInfo[INFO_POS].y)
	{
		m_pDynamicTransCom->Move_Pos(&(_vec3(0.f, 1.f, 0.f) * 0.03f));
		m_pDynamicTransCom->Update_Component(fTimeDelta);
	}

	if (m_fRearrangementDelay < m_fRearrangementDealyCount)
	{
		m_fRearrangementDealyCount = 0.f;

		switch (m_pAnimationCom->m_iMotion)
		{
		case 0:

			break;
		case 1:
			m_pAnimationCom->m_iMotion = 0;
			break;
		case 2:
			m_pAnimationCom->m_iMotion = 1;
			break;
		case 3:
			m_pAnimationCom->m_iMotion = 2;
			break;

		case 4:
			if (m_bLeftRight)
				m_pAnimationCom->m_iMotion = 3;
			else
				m_pAnimationCom->m_iMotion = 5;
			break;

		case 5:
			m_pAnimationCom->m_iMotion = 6;
			break;
		case 6:
			m_pAnimationCom->m_iMotion = 7;
			break;
		case 7:
			m_pAnimationCom->m_iMotion = 0;
		}
	}
}

bool		CSphinxFlyHead::Dead_Judge(const _float& fTimeDelta)
{
	if (0 >= m_pInfoCom->Get_Hp())
	{
		m_bDead = true;
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		m_fDeadY = vPos.y;
		//Safe_Release(m_pAttackAnimationCom);
	}
	if (m_bDead)
	{
		Dead_Action(fTimeDelta);

		CScene* pScene = ::Get_Scene();
		NULL_CHECK_RETURN(pScene, false);

		static_cast<CStage*>(pScene)->Set_SphinxHeadBGM(true);
	
		m_pDynamicTransCom->Update_Component(fTimeDelta);
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);
		return true;
	}
	else
	{
		return false;
	}
}

void		CSphinxFlyHead::Dead_Action(const _float& fTimeDelta)
{
	if (m_pDeadAnimationCom->m_iMotion < m_pDeadAnimationCom->m_iMaxMotion)
	{
		m_pDeadAnimationCom->Move_Animation(fTimeDelta);

	}
	//23
	//17 20 23
	
	if (12 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[0])
		{
			m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_bArrFalldown[0] = true;
		}
	}
	else if (13 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[1])
		{
			m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_bArrFalldown[1] = true;
		}
	}
	else if (14 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[2])
		{
			m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_bArrFalldown[2] = true;
		}
	}
	else if (15 == m_pDeadAnimationCom->m_iMotion)
	{
		if (false == m_bArrFalldown[3])
		{
			//m_pDynamicTransCom->Add_Y(-m_fDeadY * 0.2f);
			m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y*0.5f);
			//m_pDynamicTransCom->Set_Y(5.f);
			m_bArrFalldown[3] = true;
		}
		
		//폭죽 올라가서 터지는 효과
	}
	if (23 == m_pDeadAnimationCom->m_iMotion)
	{
	m_fFlareTimeCount -= fTimeDelta;
	if (m_fFlareTimeCount < 0 && m_iFlareCount < 4)
	{
		
			CScene* pScene = ::Get_Scene();
			CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

			CGameObject* pGameObject = nullptr;
			switch (m_iFlareCount)
			{
			case 0:
				pGameObject = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS]);


				pMyLayer->Add_EffectList(pGameObject);
				m_iFlareCount++;
				m_fFlareTimeCount = 0.7f;
				break;
			case 1:
				pGameObject = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS] + _vec3(-4.f, 0.f, -4.f));


				pMyLayer->Add_EffectList(pGameObject);
				m_iFlareCount++;
				m_fFlareTimeCount = 0.7f;
				break;
			case 2:
				pGameObject = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS] + _vec3(4.f, 0.f, 4.f));


				pMyLayer->Add_EffectList(pGameObject);
				m_iFlareCount++;
				m_fFlareTimeCount = 0.7f;
				break;
			default:
				pGameObject = CFlare::Create(m_pGraphicDev, m_pDynamicTransCom->m_vInfo[INFO_POS]);


				pMyLayer->Add_EffectList(pGameObject);
				m_iFlareCount++;
				m_fFlareTimeCount = 0.7f;
				break;
			}
			
		}
	}
}

void		CSphinxFlyHead::Add_HpBar()
{
	if (false == m_bHpBarCreated)
	{
		CScene* pScene = ::Get_Scene();
		CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

		CGameObject* pHpBar = nullptr;
		pHpBar = CMonsterHpBar::Create(m_pGraphicDev, m_pDynamicTransCom, m_pInfoCom, m_pDynamicTransCom->m_vInfo[INFO_POS].x, m_pDynamicTransCom->m_vInfo[INFO_POS].z);

		pMyLayer->Add_EffectList(pHpBar);
		m_bHpBarCreated = true;
	}
}


CSphinxFlyHead * CSphinxFlyHead::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy, float Size)
{
	CSphinxFlyHead*	pInstance = new CSphinxFlyHead(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, Size)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSphinxFlyHead::Free(void)
{
	CMonsterBase::Free();
}
