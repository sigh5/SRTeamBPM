#include "..\..\Header\EquipmentBase.h"
#include "Export_Function.h"

#include "Calculator.h"

#include "Texture.h"
#include "RcTex.h"
#include "Animation.h"
#include "Transform.h"

#include "Export_Utility.h"

USING(Engine)

CEquipmentBase::CEquipmentBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev) , m_EquipState(EquipState_Equip_End)
{
	ZeroMemory(&rcUI, sizeof(RECT));
	
}

CEquipmentBase::CEquipmentBase(const CEquipmentBase & rhs)
	: CGameObject(rhs), m_EquipState(rhs.m_EquipState)
{
	memcpy(&rcUI,&rhs.rcUI, sizeof(RECT));
}

CEquipmentBase::~CEquipmentBase()
{
}

HRESULT CEquipmentBase::Ready_Object(void)
{
	return S_OK;
}

_int CEquipmentBase::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CEquipmentBase::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

HRESULT CEquipmentBase::Ready_EquipInfo(_uint iPlusAtk, _uint iPlusDefense, _float fPlusSpeed, _float fPlusRange, WEAPON_TYPE eID)
{
	memcpy(&m_EquipInfo._iAddAttack, &iPlusAtk, sizeof(_uint));
	memcpy(&m_EquipInfo._iAddDefense, &iPlusDefense, sizeof(_uint));
	memcpy(&m_EquipInfo._fAddSpeed, &fPlusSpeed, sizeof(_float));
	memcpy(&m_EquipInfo._fAddRange, &fPlusRange, sizeof(_float));
	memcpy(&m_EquipInfo.m_WeaponType, &eID, sizeof(WEAPON_TYPE));

	return S_OK;
}


HRESULT CEquipmentBase::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	pComponent = m_pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(Clone_Proto(L"Proto_DynamicTransformCom"));
	NULL_CHECK_RETURN(m_pDynamicTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_DynamicTransformCom" , pComponent });
	
	pComponent = m_pOrthoTransCom = dynamic_cast<COrthoTransform*>(Clone_Proto(L"Proto_OrthoTransformCom"));
	NULL_CHECK_RETURN(m_pOrthoTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_OrthoTransformCom" , pComponent });


	return S_OK;
}

void CEquipmentBase::Free(void)
{
	CGameObject::Free();
}
