#include "stdafx.h"
#include "..\Header\Ghul.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Player.h"

#include "MyCamera.h"
#include "Gun_Screen.h"

CGhul::CGhul(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CGhul::~CGhul()
{
}

HRESULT CGhul::Ready_Object(int Posx, int Posy)
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
	m_pInfoCom->Ready_CharacterInfo(5, 10, 1.f);

	m_iMonsterIndex = 6;
	m_fHitDelay = 0.f;
	m_iMonsterIndex = 6;
	m_fAttackDelay = 0.5f;

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((float)Posx, 1.f, (float)Posy);
	}
	m_pDynamicTransCom->Update_Component(1.f);
	return S_OK;
}

_int CGhul::Update_Object(const _float & fTimeDelta)
{
	DigOut(fTimeDelta);
	if (Dead_Judge(fTimeDelta))
	{
		return 0;
	}

	m_fTimeDelta = fTimeDelta;

	Hit_Delay_toZero();

	AttackJudge(fTimeDelta);
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);


	if (m_bHit == false)
	{
		NoHit_Loop(fTimeDelta);
	}
	else
	{
		Hit_Loop(fTimeDelta);
	}

	Excution_Event();



	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CGhul::LateUpdate_Object(void)
{
	// 빌보드 에러 해결
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
		D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

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
	}
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
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//walk false일때 dig

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
				m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
			}	// 텍스처 정보 세팅을 우선적으로 한다.
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


	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() &&
		fMtoPDistance < MAX_CROSSROAD  &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(1);
		cout << "Spider " << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);
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
	if (fMtoPDistance > 6.f && m_bAttacking == false)
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
		if (2.5f > Distance)
		{
			pPlayerInfo->Receive_Damage(m_pInfoCom->Get_AttackPower());
		}
	}
	if (9 == m_pAttackAnimationCom->m_iMotion)
	{
		if (2.5f > Distance)
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

CGhul * CGhul::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
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
