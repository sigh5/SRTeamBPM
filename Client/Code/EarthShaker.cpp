#include "stdafx.h"
#include "..\Header\EarthShaker.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "EarthSpike.h"
#include "HitEffect.h"
#include "Player.h"
#include "Gun_Screen.h"

CEarthShaker::CEarthShaker(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CEarthShaker::~CEarthShaker()
{
}

HRESULT CEarthShaker::Ready_Object(float Posx, float Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CComponent* pComponent = nullptr;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_EarthShaker_Texture", m_mapComponent, ID_STATIC);
	m_pAttackTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_EarthShaker_Attack_Texture", m_mapComponent, ID_STATIC);
	m_pDeadTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_EarthShaker_Dead_Texture", m_mapComponent, ID_STATIC);
	m_pAttackAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = MONSTER_EARTHSHAKER;
	m_pAttackAnimationCom->Ready_Animation(8, 0, 0.8f);

	m_pAnimationCom->Ready_Animation(7, 1, 0.4f);
	
	m_pDeadAnimationCom->Ready_Animation(8, 0, 0.4f);
	
	m_pInfoCom->Ready_CharacterInfo(4, 10, 3.f);

	m_fAttackDelay = 0.3f;
	m_bDead = false;
	m_fHitDelay = 0.f;
	m_fInterval = 1.9f;

	m_fWaitingTime = 0.35f;

	m_pDynamicTransCom->Set_Scale(&_vec3(6.f, 7.f, 6.f));

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos((_float)Posx, m_pDynamicTransCom->m_vScale.y * 0.5f, (_float)Posy);
	}
	Save_OriginPos();
	m_pDynamicTransCom->Update_Component(1.f);

	return S_OK;
}

_int CEarthShaker::Update_Object(const _float & fTimeDelta)
{

	for (auto iter = m_Spikelist.begin(); iter != m_Spikelist.end();)
	{
		_int iResult = 0;
		iResult = (*iter)->Update_Object(fTimeDelta);
		if (iResult == 1)
		{
			Safe_Release((*iter));
			iter = m_Spikelist.erase(iter);
		}
		else
		{
			++iter;
		}
	}
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
	m_pDynamicTransCom->Update_Component(fTimeDelta);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CEarthShaker::LateUpdate_Object(void)
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
	Engine::CMonsterBase::LateUpdate_Object();
}

void CEarthShaker::Render_Obejct(void)
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

void CEarthShaker::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));
	_vec3	vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	_vec3 PickPos;

	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot() == true &&
		fMtoPDistance < MAX_CROSSROAD &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{
		m_bHit = true;
		static_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"))->Set_ComboCount(1);
		m_pInfoCom->Receive_Damage(1);
		cout << "FatBat" << m_pInfoCom->Get_InfoRef()._iHp << endl;
		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_EARTHSHAKER, 0, 8, 0.2f);
	}

}

bool CEarthShaker::Dead_Judge(const _float & fTimeDelta)
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

void		CEarthShaker::AttackJudge(const _float& fTimeDelta)
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

	if (m_bReady_Attack)
	{
		m_bAttacking = true;
	}
	else
	{
		m_bAttacking = false;
	}
}
void		CEarthShaker::Attack(const _float& fTimeDelta)
{
	Ready_Attack(fTimeDelta);
	m_pAttackAnimationCom->Move_Animation(fTimeDelta);
	if (m_pAttackAnimationCom->m_iMotion == 2)
	{
		if (0 < m_iReadyAttackNumber)
		{
			m_pAttackAnimationCom->m_iMotion = 0;
			--m_iReadyAttackNumber;
			++m_iAttacknumber;
		}
		if (m_iReadyAttackNumber == 0)
		{
			if (m_bAttackWaiting == false)
			{
				m_pAttackAnimationCom->m_iMotion = 1;
				m_bAttackWaiting = true;
			}
		}
	}
	if (m_pAttackAnimationCom->m_iMotion == 4)
	{
		if (false == m_bSpikeCreate)
		{
			_vec3 ShakerPos, vDir;
			m_pDynamicTransCom->Get_Info(INFO_POS, &ShakerPos);
			vDir = m_vPlayerPos - ShakerPos;
			D3DXVec3Normalize(&vDir, &vDir);

			CEarthSpike* pSpike;
			for (int i = 1; i < 12; ++i)
			{
				pSpike = CEarthSpike::Create(m_pGraphicDev, m_fWaitingTime * i, ShakerPos.x + vDir.x * m_fInterval * i, ShakerPos.z + vDir.z * m_fInterval * i, m_bSpikeType);
				m_Spikelist.push_back(pSpike);
				if (m_bSpikeType)
				{
					m_bSpikeType = false;
				}
				else
				{
					m_bSpikeType = true;
				}
			}
			m_bSpikeCreate = true;
			--m_iAttacknumber;
		}
		//땅을 찍는 순간 플레이어의 점프 상태를 검사

		if (0 < m_iAttacknumber)
		{
			m_pAttackAnimationCom->m_iMotion = 2;
			m_bSpikeCreate = false;
		}
	}
	if (m_pAttackAnimationCom->m_iMotion == 7)
	{
		if (0 < m_iDefenseless)
		{
			m_pAttackAnimationCom->m_iMotion = 6;
			--m_iDefenseless;
		}
	}
	if (m_pAttackAnimationCom->m_iMotion == m_pAttackAnimationCom->m_iMaxMotion)
	{
		m_bAttack = false;
		m_bReady_Attack = false;
		m_bAttackWaiting = false;
		m_iAttacknumber = 0;
		m_iReadyAttackNumber = 0;
	}

	//팔을 치켜 올린 만큼 공격

}

void	CEarthShaker::Ready_Attack(const _float& fTimeDelta)
{
	if (m_bReady_Attack == false)
	{
		m_bReady_Attack = true;
		m_iReadyAttackNumber = 1 + rand() % 3;

		m_iDefenseless = m_iReadyAttackNumber;
	}
}
void CEarthShaker::NoHit_Loop(const _float& fTimeDelta)
{
	// 거리
	if (20.f <  fMtoPDistance && m_bAttacking == false)
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

CEarthShaker * CEarthShaker::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy)
{
	CEarthShaker*	pInstance = new CEarthShaker(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEarthShaker::Free(void)
{
	CMonsterBase::Free();
	for (auto& iter : m_Spikelist)
	{
		Safe_Release(iter);
	}
	m_Spikelist.clear();
}
