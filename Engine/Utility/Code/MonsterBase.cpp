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
	:CGameObject(pGraphicDev)
{
}

CMonsterBase::CMonsterBase(const CMonsterBase & rhs)
	: CGameObject(rhs)
{
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
	NULL_CHECK_RETURN(pTerrainBufferCom, false );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, false);

	_vec3 Temp = _pCalcul->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pDynamicTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

	return false;
}

CharacterInfo&	CMonsterBase::Get_InfoRef()
{
	return m_pInfoCom->Get_InfoRef();
}

void CMonsterBase::Get_MonsterToPlayer_Distance(float* _Distance)
{
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);
	m_pDynamicTransCom->Get_Info(INFO_POS, &m_vMonsterPos);

	float fMtoPDistance; // 몬스터와 플레이어 간의 거리

	fMtoPDistance = sqrtf((powf(m_vMonsterPos.x - m_vPlayerPos.x, 2) + powf(m_vMonsterPos.y - m_vPlayerPos.y, 2) + powf(m_vMonsterPos.z - m_vPlayerPos.z, 2)));

	memcpy(_Distance, &fMtoPDistance, sizeof(float));
}


void Engine::CMonsterBase::Free(void)
{
	CGameObject::Free();
}