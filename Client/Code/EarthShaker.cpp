#include "stdafx.h"
#include "..\Header\EarthShaker.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"

CEarthShaker::CEarthShaker(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CEarthShaker::~CEarthShaker()
{
}

HRESULT CEarthShaker::Ready_Object(float Posx, float Posy, float Size)
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

	if (Size != 0)
	{
		m_pDynamicTransCom->Set_Scale(&_vec3(Size, Size, Size));
	}
	else
	{
		m_pDynamicTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	}
	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pDynamicTransCom->Set_Pos(Posx, 3.f, Posy);
	}
	m_pDynamicTransCom->Update_Component(1.f);
	Save_OriginPos();
	return S_OK;
}

_int CEarthShaker::Update_Object(const _float & fTimeDelta)
{

	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	if (Distance_Over())
	{
		Engine::CMonsterBase::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_ALPHA, this);

		return 0;
	}
	if (Dead_Judge(fTimeDelta)) //깡통
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
	if (m_pAttackAnimationCom->m_iMotion == 5)
	{
		//땅을 찍는 순간 플레이어의 점프 상태를 검사

		if (0 < m_iAttacknumber)
		{
			m_pAttackAnimationCom->m_iMotion = 2;
			--m_iAttacknumber;
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
	}

	//팔을 치켜 올린 만큼 공격

}

void	CEarthShaker::Ready_Attack(const _float& fTimeDelta)
{
	if (m_bReady_Attack == false)
	{
		m_bReady_Attack = true;
		m_iAttacknumber = 1 + rand() % 3;
		m_iReadyAttackNumber = m_iAttacknumber - 1;
		m_iDefenseless = m_iReadyAttackNumber;
	}
}
void CEarthShaker::NoHit_Loop(const _float& fTimeDelta)
{
	if (fMtoPDistance > 10.f && m_bAttacking == false)
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

CEarthShaker * CEarthShaker::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy, float Size)
{
	CEarthShaker*	pInstance = new CEarthShaker(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, Size)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEarthShaker::Free(void)
{
	CMonsterBase::Free();
}
