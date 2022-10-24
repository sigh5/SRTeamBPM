#include "stdafx.h"
#include "..\Header\Spider.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"

#include "Gun_Screen.h"
#include "HitEffect.h"
#include "Special_Effect.h"
#include "Key.h"
#include "Coin.h"

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CSpider::~CSpider()
{
}

HRESULT CSpider::Ready_Object(int Posx, int Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CComponent* pComponent = nullptr;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MonsterTexture3", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Spider_Attack_Texture", m_mapComponent, ID_STATIC);
	m_pAttackAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Spider_Dead_Texture", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = MONSTER_SPIDER;
	m_fAttackDelay = 0.3f;
	m_pInfoCom->Ready_CharacterInfo(50, 10, 7.f);
	m_pAnimationCom->Ready_Animation(4, 1, 0.07f);
	m_pDeadAnimationCom->Ready_Animation(13, 0, 0.2f);
	m_pAttackAnimationCom->Ready_Animation(13, 0, 0.2f);
	m_fHitDelay = 0.f;
	m_pDynamicTransCom->Set_Scale(&_vec3(3.f, 3.f, 3.f));
	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((_float)Posx, m_pDynamicTransCom->m_vScale.y * 0.5f, (_float)Posy);
	}
	Save_OriginPos();

	// ControlRoom
	_vec3 vPos, vScale;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	// ~ControlRoom
	return S_OK;
}
bool	CSpider::Dead_Judge(const _float& fTimeDelta)
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
				::PlaySoundW(L"Spider_death_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_death_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_death_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
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
_int CSpider::Update_Object(const _float & fTimeDelta)
{
	// 맨위에있어야됌 리턴되면 안됌
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	// 맨위에있어야됌 리턴되면 안됌

	//쿨타임 루프
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
	m_fTimeDelta = fTimeDelta;

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

void CSpider::LateUpdate_Object(void)
{


	// 빌보드 에러 해결
	/*CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);*/

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

void CSpider::Render_Obejct(void)
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

void CSpider::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));
	_vec3	vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot()&&
		fMtoPDistance < MAX_CROSSROAD + g_fRange  &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(pPlayerInfo->Get_AttackPower());
		cout << "Spider "<<m_pInfoCom->Get_InfoRef()._iHp << endl;
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
				::PlaySoundW(L"Spider_pain_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;

			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_pain_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;

			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_pain_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			}
		}
	}
}

void CSpider::Excution_Event(_bool bAOE)
{

	if (bAOE)
	{
		m_pInfoCom->Receive_Damage(1);
		return;
	}


	if (!m_bDead &&  1 >= m_pInfoCom->Get_Hp())
	{
		m_pInfoCom->Receive_Damage(1);
		_vec3	vPos;
		CGameObject *pGameObject = nullptr;
		CScene* pScene = Get_Scene();
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		READY_CREATE_EFFECT_VECTOR(pGameObject, CSpecial_Effect, pLayer, m_pGraphicDev, vPos);
		static_cast<CSpecial_Effect*>(pGameObject)->Set_Effect_INFO(OWNER_PALYER, 0, 17, 0.2f);

		::PlaySoundW(L"explosion_1.wav", SOUND_EFFECT, g_fSound); // BGM

	}
}

void		CSpider::Attack(const _float& fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pAttackAnimationCom->Move_Animation(fTimeDelta);

	if (false == m_bReadyAttackSound)
	{
		_int Hitsound = rand() % 3;
		switch (Hitsound)
		{
		case 0:
			::StopSound(SOUND_MONSTER);
			::PlaySoundW(L"Spider_detect_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
			break;
		case 1:
			::StopSound(SOUND_MONSTER);
			::PlaySoundW(L"Spider_detect_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
			break;
		case 2:
			::StopSound(SOUND_MONSTER);
			::PlaySoundW(L"Spider_detect_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
			break;
		}
		m_bReadyAttackSound = true;
	}

	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	float Distance;
	Get_MonsterToPlayer_Distance(&Distance);
	if (6==m_pAttackAnimationCom->m_iMotion)
	{
		if (7.f > Distance && m_bHitDamage)
		{
			pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
			pPlayer->Set_DefenseToHp(true);
			m_bHitDamage = false;
		}
		if (false == m_bAttackSound)
		{
			_int Hitsound = rand() % 3;
			switch (Hitsound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_attack_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_attack_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_attack_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			}
			m_bAttackSound = true;
		}
	}
	
	if (9 == m_pAttackAnimationCom->m_iMotion)
	{
		if (7.f > Distance && m_bHitDamage)
		{
			pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
			pPlayer->Set_DefenseToHp(true);
			m_bHitDamage = false;
		}
		if (false == m_bAttackSound2)
		{
			_int Hitsound = rand() % 3;
			switch (Hitsound)
			{
			case 0:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_attack_01.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 1:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_attack_02.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			case 2:
				::StopSound(SOUND_MONSTER);
				::PlaySoundW(L"Spider_attack_03.wav", SOUND_MONSTER, g_fSound * 1.5f);
				break;
			}
			m_bAttackSound2 = true;
		}
	}

	if (m_pAttackAnimationCom->m_iMotion >= m_pAttackAnimationCom->m_iMaxMotion)
	{
		m_bAttack = false;
		m_bHitDamage = true;
		m_bAttackSound = false;
		m_bAttackSound2 = false;
	}
}

void		CSpider::AttackJudge(const _float& fTimeDelta)
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

void CSpider::NoHit_Loop(const _float& fTimeDelta)
{
	 // 일정 거리 이하면 추적을 안함 // 근데 공격모션 오류 있는거 수정부탁드립니다.


	if (fMtoPDistance > 5.5f && m_bAttacking == false)
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
void CSpider::Hit_Loop(const _float& fTimeDelta)
{
	m_pAnimationCom->m_iMotion = 5;

	m_fHitDelay += fTimeDelta;
	if (m_fHitDelay > 1.5f)
	{
		m_bHit = false;
		m_fHitDelay = 0.f;
	}
}

void	CSpider::Drop_Item(int ItemType)
{
	CScene  *pScene = ::Get_Scene();
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject* pItem = nullptr;
	switch (ItemType)
	{
	case 0:
		pItem = CCoin::Create(m_pGraphicDev, (_int)m_pDynamicTransCom->m_vInfo[INFO_POS].x, (_int)m_pDynamicTransCom->m_vInfo[INFO_POS].z);
		pLayer->Add_DropItemList(pItem);
		break;

	case 1:
		pItem = CKey::Create(m_pGraphicDev, (_int)m_pDynamicTransCom->m_vInfo[INFO_POS].x, (_int)m_pDynamicTransCom->m_vInfo[INFO_POS].z);
		pLayer->Add_DropItemList(pItem);
		break;

	default:
		break;
	}
}
HRESULT CSpider::SetUp_Material(void)
{
	if (Get_Distance() < 30.f)
	{
		_float fDistance = ((1.f - Get_Distance() / 30.f)) * 2;
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
void CSpider::Set_Light_Obj()
{
	if (Get_Distance() >= 31.f)
	{
		D3DLIGHT9		tLightInfo4;
		ZeroMemory(&tLightInfo4, sizeof(D3DLIGHT9));
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo4, 3), );
		return;
	}

	if (Get_Distance() < 30.f)
	{
		_float fDistance = ((1.f - Get_Distance() / 30.f))*2.f;;
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
CSpider * CSpider::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CSpider*	pInstance = new CSpider(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSpider::Free(void)
{
	CMonsterBase::Free();
}

