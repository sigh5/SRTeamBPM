#include "stdafx.h"
#include "..\Header\Spider.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"

#include "Gun_Screen.h"

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
	m_pAttackAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_DYNAMIC);


	m_iMonsterIndex = 2;
	m_fAttackDelay = 0.3f;
	m_pInfoCom->Ready_CharacterInfo(100, 10, 8.f);
	m_pAnimationCom->Ready_Animation(4, 1, 0.07f);
	m_pAttackAnimationCom->Ready_Animation(13, 0, 0.2f);
	m_iPreHp = (m_pInfoCom->Get_InfoRef()._iHp);
	if (Posx == 0 && Posy == 0) {}
	
	else
	{
		Set_TransformPositon(g_hWnd, m_pCalculatorCom);
	}

	return S_OK;
}

_int CSpider::Update_Object(const _float & fTimeDelta)
{
	//쿨타임 루프

	m_fTimeDelta = fTimeDelta;

	if (m_iPreHp > m_pInfoCom->Get_Hp())
	{
		m_iPreHp = m_pInfoCom->Get_Hp();
		m_bHit = true;
		m_bAttacking = false;
		if (m_fHitDelay != 0)
		{
			m_fHitDelay = 0;
		}
	}

	//AttackJudge(fTimeDelta);
	////~

	//CDynamic_Transform*		pPlayerTransformCom = dynamic_cast<CDynamic_Transform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	//NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	////Set_OnTerrain();
	//float TerrainY = m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom);
	//m_pDynamicTransCom->Set_Y(TerrainY + 1.f);
	////지형에 올림

	//_vec3		vPlayerPos, vMonsterPos;
	//pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	//m_pDynamicTransCom->Get_Info(INFO_POS, &vMonsterPos);

	//
	//Get_MonsterToPlayer_Distance(&fMtoPDistance);

	AttackJudge(fTimeDelta);
	CMonsterBase::Get_MonsterToPlayer_Distance(&fMtoPDistance);
	
	if (m_bHit == false)
	{
		if (fMtoPDistance > 3.f && m_bAttacking == false)
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
	else
	{
		// 피격 시 모션
		m_pAnimationCom->m_iMotion = 5;
	}
	

	if (m_bHit)
	{
		m_fHitDelay += fTimeDelta;
		if (m_fHitDelay > 1.5f)
		{

			m_bHit = false;
			m_fHitDelay = 0.f;
		}
	}

	// 처형이벤트
	Excution_Event();
	
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
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

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

void CSpider::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	if (m_bAttacking)
	{
		m_pAttackTextureCom->Set_Texture(m_pAttackAnimationCom->m_iMotion);
	}
	else
	{
	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
	}

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CSpider::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;
	pGameObject = static_cast<CGun_Screen*>(::Get_GameObject(L"Layer_UI", L"Gun"));


	if (static_cast<CGun_Screen*>(pGameObject)->Get_Shoot()&&
		fMtoPDistance < MAX_CROSSROAD  &&
		m_pColliderCom->Check_Lay_InterSect(m_pBufferCom, m_pDynamicTransCom, g_hWnd))
	{

		static_cast<CPlayer*>(pGameObject)->Set_ComboCount(1);

		m_pInfoCom->Receive_Damage(1);
		cout << "Spider "<<m_pInfoCom->Get_InfoRef()._iHp << endl;
		static_cast<CGun_Screen*>(pGameObject)->Set_Shoot(false);
	}



}

void CSpider::Excution_Event()
{
	// 나중에 로직추가
}

void		CSpider::Attack(const _float& fTimeDelta)
{
	m_pAttackAnimationCom->Move_Animation(fTimeDelta);

	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	float Distance;
	Get_MonsterToPlayer_Distance(&Distance);
	if (6==m_pAttackAnimationCom->m_iMotion)
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
	CGameObject::Free();
}

