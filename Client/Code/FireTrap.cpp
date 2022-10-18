#include "stdafx.h"
#include "..\Header\FireTrap.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "FirePillar.h"
#include "Player.h"
#include "HitEffect.h"

#include "MyCamera.h"
#include "Gun_Screen.h"
CFireTrap::CFireTrap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CFireTrap::~CFireTrap()
{
}

HRESULT CFireTrap::Ready_Object(float Posx, float Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_FireTrap_Texture", m_mapComponent, ID_STATIC);
	
	m_pDynamicTransCom->Set_Pos(Posx, 0.1f, Posy);
	Save_OriginPos();
	m_pInfoCom->Ready_CharacterInfo(2, 5, 0.f);
	m_iPreHp = (m_pInfoCom->Get_InfoRef()._iHp);
	m_fAttackDelay = 3.f;

	m_pDynamicTransCom->Update_Component(1.f);
	return S_OK;
}

_int CFireTrap::Update_Object(const _float & fTimeDelta)
{
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	for (auto iter = m_FirePillarlist.begin(); iter != m_FirePillarlist.end();)
	{
		_int iResult = 0;
		iResult = (*iter)->Update_Object(fTimeDelta);
		if (iResult == 1)
		{
			Safe_Release((*iter));
			iter = m_FirePillarlist.erase(iter);
		}
		else
		{
			++iter;
		}
	}
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


	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);


	m_pDynamicTransCom->Update_Component(fTimeDelta);
	Engine::CMonsterBase::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CFireTrap::LateUpdate_Object(void)
{
	_matrix      matScale, matTrans, matRot;
	D3DXMatrixScaling(&matScale, m_pDynamicTransCom->m_vScale.x, m_pDynamicTransCom->m_vScale.y, m_pDynamicTransCom->m_vScale.z);


	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationX(&matRot, (90.f*3.14f / 180.f));

	_matrix		matWorld;
	D3DXMatrixIdentity(&matWorld);

	matWorld = matScale * matRot * matTrans;
	m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));
	Engine::CMonsterBase::LateUpdate_Object();
	// 빌보드 에러 해결
}




void CFireTrap::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CFireTrap::Collision_Event()
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
		cout << "Anubis " << m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);
		READY_CREATE_EFFECT_VECTOR(pGameObject, CHitEffect, pLayer, m_pGraphicDev, vPos);
		static_cast<CHitEffect*>(pGameObject)->Set_Effect_INFO(OWNER_ANUBIS, 0, 7, 0.2f);
	}
}

bool CFireTrap::Dead_Judge(const _float & fTimeDelta)
{
	if (0 >= m_pInfoCom->Get_Hp())
	{
		if (false == m_bDead)
		{
			m_bDead = true;
		}
		//Safe_Release(m_pAttackAnimationCom);
	}
	if (m_bDead)
	{

		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);
		return true;
	}
	else
	{
		return false;
	}
}

void CFireTrap::NoHit_Loop(const _float & fTimeDelta)
{
	if (2.5f < fMtoPDistance  && m_bAttacking == false)
	{

	}
	else
	{
		if (m_bAttack)
		{
			Attack(fTimeDelta);
		}
		else
		{
			
		}
	}
}

void CFireTrap::Hit_Loop(const _float & fTimeDelta)
{
}

void CFireTrap::AttackJudge(const _float & fTimeDelta)
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

void CFireTrap::Attack(const _float & fTimeDelta)
{
	_vec3 TrapPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &TrapPos);
	m_pAnimationCom->m_iMotion = 1;

	CFirePillar* pFire;
	pFire = CFirePillar::Create(m_pGraphicDev, TrapPos.x, TrapPos.y, TrapPos.z);

	m_FirePillarlist.push_back(pFire);
	m_bAttack = false;
	//불기둥 생성
}

CFireTrap * CFireTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy)
{
	CFireTrap*	pInstance = new CFireTrap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFireTrap::Free(void)
{
	CMonsterBase::Free();

	for (auto& iter : m_FirePillarlist)
	{
		Safe_Release(iter);
	}
	m_FirePillarlist.clear();
}
