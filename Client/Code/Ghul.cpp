#include "stdafx.h"
#include "..\Header\Ghul.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Player.h"

#include "MyCamera.h"
#include "Gun_Screen.h"


#include "HitEffect.h"



CGhul::CGhul(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CGhul::~CGhul()
{
}

HRESULT CGhul::Ready_Object(float Posx, float Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Ghul_Texture", m_mapComponent, ID_STATIC);
	m_pGhulDigoutTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Ghul_dig_Texture", m_mapComponent, ID_STATIC);
	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Ghul_Dead_Texture", m_mapComponent, ID_STATIC);
	m_pAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Ghul_Attack_Texture", m_mapComponent, ID_STATIC);

	m_pDigoutAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pDigoutAnimationCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Ghul_dig_AnimationCom", m_pDigoutAnimationCom });

	m_pAttackAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_pDigoutAnimationCom->Ready_Animation(17, 0, 0.3f);
	m_pAttackAnimationCom->Ready_Animation(13, 0, 0.2f);
	m_pDeadAnimationCom->Ready_Animation(11, 0, 0.2f);
	m_pInfoCom->Ready_CharacterInfo(1, 10, 1.f);

	m_iMonsterIndex = MONSTER_GHUL;
	m_fHitDelay = 0.f;
	m_fAttackDelay = 0.5f;
	m_pDynamicTransCom->Set_Scale(&_vec3(4.f, 4.f, 4.f));
	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((float)Posx, m_pDynamicTransCom->m_vScale.y * 0.5f, (float)Posy);
	}
	m_pDynamicTransCom->Update_Component(1.f);
	Save_OriginPos();
	
	// ControlRoom
	_vec3 vPos, vScale;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
	vScale =  m_pDynamicTransCom->Get_Scale();
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	// ~ControlRoom
	
	return S_OK;
}

_int CGhul::Update_Object(const _float & fTimeDelta)
{
	m_pDynamicTransCom->Set_Y(m_pDynamicTransCom->m_vScale.y * 0.5f);
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	if (Distance_Over())
	{
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}
	DigOut(fTimeDelta);
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

	Excution_Event();

	// Cotrol Room
	_matrix matWorld;
	_vec3 vScale;
	vScale= m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld,vScale);
	// ~Cotrol Room

	m_pDynamicTransCom->Update_Component(fTimeDelta);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CGhul::LateUpdate_Object(void)
{
	// ������ ���� �ذ�
	if (SCENE_TOOLTEST != Get_Scene()->Get_SceneType())
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

		// ������ ���� �ذ�
	}
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
	pMyLayer->Add_vecColliderMonster(static_cast<CMonsterBase*>(this));
	Engine::CMonsterBase::LateUpdate_Object();
}

void CGhul::Render_Obejct(void)
{
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//walk false�϶� dig

	if (false == m_bDead)
	{
		if (true == m_bCanWalk)
		{
			if (m_bAttacking && false == m_bHit)
			{
				m_pAttackTextureCom->Set_Texture(m_pAttackAnimationCom->m_iMotion);
			}
			else
			{
				m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// �ؽ�ó ���� ������ �켱������ �Ѵ�.
			}	// �ؽ�ó ���� ������ �켱������ �Ѵ�.
		}
		else
		{
			m_pGhulDigoutTextureCom->Set_Texture(m_pDigoutAnimationCom->m_iMotion);
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

void CGhul::Collision_Event()
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
		cout << "Ghul" << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);

		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_GHUL, 0, 8, 0.2f);

	}
}

bool CGhul::Dead_Judge(const _float & fTimeDelta)
{
	if (0 >= m_pInfoCom->Get_Hp())
	{
		m_bDead = true;
		//Safe_Release(m_pAttackAnimationCom);
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

void CGhul::NoHit_Loop(const _float & fTimeDelta)
{
	if(m_bCanWalk)
	if (6.f < fMtoPDistance  && m_bAttacking == false)
	{
		m_pDynamicTransCom->Chase_Target_notRot(&m_vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);

		m_pAnimationCom->Move_Animation(fTimeDelta);

	}
	else
	{
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

void CGhul::Hit_Loop(const _float & fTimeDelta)
{
	m_pAnimationCom->m_iMotion = 7;
	m_fHitDelay += fTimeDelta;
	if (m_fHitDelay > 1.5f)
	{
		m_bHit = false;
		m_fHitDelay = 0.f;
	}
}

void CGhul::AttackJudge(const _float & fTimeDelta)
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
	if (false == m_bHit)
	{
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
	else
		m_pAttackAnimationCom->m_iMotion = 0;
}

void CGhul::Attack(const _float & fTimeDelta)
{
	m_pAttackAnimationCom->Move_Animation(fTimeDelta);

	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	float Distance;
	Get_MonsterToPlayer_Distance(&Distance);

	if (6 == m_pAttackAnimationCom->m_iMotion)
	{
		if (7.f > Distance)
		{
			pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
		}
	}
	if (9 == m_pAttackAnimationCom->m_iMotion)
	{
		if (7.f > Distance)
		{
			pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
		}
	}

	if (m_pAttackAnimationCom->m_iMotion >= m_pAttackAnimationCom->m_iMaxMotion)
	{
		m_bAttack = false;
	}
}

void CGhul::DigOut(const _float& fTimeDelta)
{
	m_pDigoutAnimationCom->Move_Animation(fTimeDelta);
	if (11 == m_pDigoutAnimationCom->m_iMotion)
	{
		m_bCanDie = true;
	}
	if (!m_bCanDie)
	{
		if (1 > m_pInfoCom->Get_Hp())
		{
			m_pInfoCom->Add_Hp(1 - m_pInfoCom->Get_Hp());
		}
	}
	if (m_pDigoutAnimationCom->m_iMaxMotion <= m_pDigoutAnimationCom->m_iMotion)
	{
		m_bCanWalk = true;
	}
}

CGhul * CGhul::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy)
{
	CGhul*	pInstance = new CGhul(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGhul::Free(void)
{
	CMonsterBase::Free();
}
