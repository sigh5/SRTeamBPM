#include "..\..\Header\ItemBase.h"

#include "Calculator.h"
#include "CharacterInfo.h"
#include "Texture.h"
#include "RcTex.h"
#include "Animation.h"
#include "Transform.h"

#include "Export_Utility.h"

USING(Engine)


CItemBase::CItemBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CItemBase::CItemBase(const CItemBase & rhs)
	: CGameObject(rhs)//, m_pInfoCom(rhs.m_pInfoCom), m_pAnimationCom(rhs.m_pAnimationCom), m_pDynamicTransCom(rhs.m_pDynamicTransCom)
{
}

CItemBase::~CItemBase()
{
}

HRESULT CItemBase::Ready_Object(void)
{
	return S_OK;
}

_int CItemBase::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CItemBase::LateUpdate_Object(void)
{
}

CharacterInfo & CItemBase::Get_InfoRef()
{
	return m_pInfoCom->Get_InfoRef();
}

HRESULT CItemBase::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	//m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	pComponent = m_pInfoCom = dynamic_cast<CCharacterInfo*>(Clone_Proto(L"Proto_CharacterInfoCom"));
	NULL_CHECK_RETURN(m_pInfoCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_CharacterInfoCom", pComponent });

	pComponent = m_pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(Clone_Proto(L"Proto_DynamicTransformCom"));
	NULL_CHECK_RETURN(m_pDynamicTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_DynamicTransformCom" , pComponent });

	return S_OK;
}
// ¡Ú																															// TerrainTex Component      // Transform Component
bool CItemBase::Set_TransformPos(HWND g_hWnd, CCalculator * _pCalcul, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, const _tchar* pComponentTag1, COMPONENTID eID, COMPONENTID eID1)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(pLayerTag, pObjTag, pComponentTag, eID));
	NULL_CHECK_RETURN(pTerrainBufferCom,false); // ¡Ú

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(pLayerTag, pObjTag, pComponentTag1, eID1));
	NULL_CHECK_RETURN(pTerrainTransformCom,false); // ¡Ú

	_vec3 Temp = _pCalcul->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pDynamicTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

	return true;
}

void CItemBase::Free(void)
{
	CGameObject::Free();
}
