#include "..\..\Header\MonsterBase.h"

#include "Calculator.h"
#include "CharacterInfo.h"
#include "Texture.h"
#include "RcTex.h"
#include "Animation.h"
#include "Transform.h"

#include "Export_Utility.h"

USING(Engine)

CMonsterBase::CMonsterBase(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_iMonsterIndex(MONSTER_END),
	m_bHit(false),
	m_fHitDelay(0.f),
	m_iPreHp(0),
	m_iOriginHp(0),
	m_fTimeDelta(0.f),
	m_bResetCheck(false),
	m_bExcutionCheck(false),
	m_bDead(false),
	m_iPreIndex(0),
	m_fAttackDelay(0),
	m_fAttackDelayTime(0),
	m_bAttack(true),
	m_bAttacking(false),
	fMtoPDistance(20.f)
{
	ZeroMemory(&m_vPlayerPos, sizeof(_vec3));
	ZeroMemory(&m_vMonsterPos, sizeof(_vec3));
	ZeroMemory(&m_vOriginPos, sizeof(_vec3));
	ZeroMemory(&m_vMonsterPos, sizeof(_vec3));

}

CMonsterBase::CMonsterBase(const CMonsterBase & rhs)
	: CGameObject(rhs),
	m_iMonsterIndex(rhs.m_iMonsterIndex),
	m_bHit(rhs.m_bHit),
	m_fHitDelay(rhs.m_fHitDelay),
	m_iPreHp(rhs.m_iPreHp),
	m_iOriginHp(rhs.m_iOriginHp),
	m_fTimeDelta(rhs.m_fTimeDelta),
	m_bResetCheck(rhs.m_bResetCheck),
	m_bExcutionCheck(rhs.m_bExcutionCheck),
	m_bDead(rhs.m_bDead),
	m_iPreIndex(rhs.m_iPreIndex),
	m_fAttackDelay(rhs.m_fAttackDelay),
	m_fAttackDelayTime(rhs.m_fAttackDelayTime),
	m_bAttack(rhs.m_bAttack),
	m_bAttacking(rhs.m_bAttacking),
	fMtoPDistance(rhs.fMtoPDistance)
{
	memcpy(&m_vPlayerPos, &rhs.m_vPlayerPos, sizeof(_vec3));
	memcpy(&m_vMonsterPos, &rhs.m_vPlayerPos, sizeof(_vec3));
	memcpy(&m_vOriginPos, &rhs.m_vPlayerPos, sizeof(_vec3));
	memcpy(&m_vMonsterPos, &rhs.m_vPlayerPos, sizeof(_vec3));

}


CMonsterBase::~CMonsterBase()
{
}

HRESULT CMonsterBase::Ready_Object(void)
{
	return S_OK;
}

_int CMonsterBase::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CMonsterBase::LateUpdate_Object(void)
{
}

bool CMonsterBase::Set_SelectGizmo(HWND g_hWnd, CCalculator* _pCalcul, CRcTex* _pBuffer)
{
	if (_pCalcul->PickingOnTransform_Monster(g_hWnd, _pBuffer, m_pDynamicTransCom))
		return true;

	// 스정본

	return false;
}


HRESULT CMonsterBase::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	//m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	pComponent = m_pInfoCom = dynamic_cast<CCharacterInfo*>(Clone_Proto(L"Proto_CharacterInfoCom"));
	NULL_CHECK_RETURN(m_pInfoCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CharacterInfoCom", pComponent });

	pComponent = m_pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(Clone_Proto(L"Proto_DynamicTransformCom"));
	NULL_CHECK_RETURN(m_pDynamicTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_DynamicTransformCom" , pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom" , pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom" , pComponent });


	pComponent = m_pDeadAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_Dead_AnimationCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Dead_AnimationCom" , pComponent });
	return S_OK;
}

void		CMonsterBase::Hit_Delay_toZero(void)
{
	if (m_iPreHp > m_pInfoCom->Get_Hp())
	{
		m_iPreHp = m_pInfoCom->Get_Hp();
		//m_bHit = true;
		m_bAttacking = false;
		if (m_fHitDelay != 0)
		{
			m_fHitDelay = 0;
		}
	}
}

bool CMonsterBase::Set_TransformPositon(HWND g_hWnd, CCalculator* _pCalcul)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, false);

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, false);

	_vec3 Temp = _pCalcul->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pDynamicTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

	return false;
}
bool		CMonsterBase::Distance_Over(void)
{
	if (fMtoPDistance > 50.f)
	{
		return true;
	}
	return false;
}

CharacterInfo&	CMonsterBase::Get_InfoRef()
{
	return m_pInfoCom->Get_InfoRef();
}

void CMonsterBase::Get_MonsterToPlayer_Distance(float* _Distance)
{
	if (SCENE_TOOLTEST != Get_Scene()->Get_SceneType())
	{
		CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
		NULL_CHECK(pPlayerTransformCom);

		pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);
		m_pDynamicTransCom->Get_Info(INFO_POS, &m_vMonsterPos);

		float fMtoPDistance; // 몬스터와 플레이어 간의 거리

		fMtoPDistance = sqrtf((powf(m_vMonsterPos.x - m_vPlayerPos.x, 2) + powf(m_vMonsterPos.y - m_vPlayerPos.y, 2) + powf(m_vMonsterPos.z - m_vPlayerPos.z, 2)));

		memcpy(_Distance, &fMtoPDistance, sizeof(float));
	}
}
void		CMonsterBase::Save_OriginPos(void)
{
	m_vOriginPos = m_pDynamicTransCom->m_vInfo[INFO_POS];
	m_iOriginHp = m_pInfoCom->Get_Hp();


	_vec3 vScale = m_pDynamicTransCom->m_vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pColliderCom->Set_HitRadiuos(0.5f);
	m_pColliderCom->Set_vCenter(&m_vOriginPos, &vScale);
	m_pDynamicTransCom->Set_CountMovePos(&_vec3(0.f, 0.f, 0.f));

	m_pColliderCom->Set_HitBoxMatrix(&(m_pDynamicTransCom->m_matWorld));
	m_pColliderCom->Update_Component(1.f);
	m_pDynamicTransCom->Update_Component(1.f);
}
void		CMonsterBase::Get_BackOriginPos(void)
{
	m_pDynamicTransCom->Set_Pos(m_vOriginPos.x, m_vOriginPos.y, m_vOriginPos.z);
	m_pInfoCom->Add_Hp(m_iOriginHp - m_pInfoCom->Get_Hp());
	m_bDead = false;
}
float CMonsterBase::Get_Radius(void)
{
	return m_pDynamicTransCom->m_vScale.x * 0.5f;
}
_vec3 CMonsterBase::Get_Pos(void)
{
	return m_pDynamicTransCom->m_vInfo[INFO_POS];
}
void	CMonsterBase::Add_ColliderMonsterlist(void)
{
	if (false == m_bDead)
	{
		CScene* pScene = ::Get_Scene();
		CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
		pMyLayer->Add_vecColliderMonster(static_cast<CMonsterBase*>(this));
	}
}

void	CMonsterBase::Move_Pos(_vec3 _move)
{
	m_pDynamicTransCom->Move_Pos(&_move);
}
void Engine::CMonsterBase::Free(void)
{
	CGameObject::Free();
}
