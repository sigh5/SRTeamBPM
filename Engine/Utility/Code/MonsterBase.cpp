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

bool CMonsterBase::Set_SelectGizmo(HWND g_hWnd)
{
	if (m_pCalculatorCom->PickingOnTransform_Monster(g_hWnd, m_pBufferCom, m_pTransCom))
		return true;


	return false;
}


HRESULT CMonsterBase::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	//m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });
	
	pComponent = m_pInfoCom = dynamic_cast<CCharacterInfo*>(Clone_Proto(L"Proto_CharacterInfoCom"));
	NULL_CHECK_RETURN(m_pInfoCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_CharacterInfoCom", pComponent });

	pComponent = m_pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(Clone_Proto(L"Proto_DynamicTransformCom"));
	NULL_CHECK_RETURN(m_pInfoCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_DynamicTransformCom" , pComponent });

	return S_OK;
}

bool CMonsterBase::Set_TransformPositon(HWND g_hWnd)
{

	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );


	_vec3 Temp = m_pCalculatorCom->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

}

CharacterInfo&	CMonsterBase::Get_InfoRef()
{
	return m_pInfoCom->Get_InfoRef();
}


void Engine::CMonsterBase::Free(void)
{




	CGameObject::Free();
}