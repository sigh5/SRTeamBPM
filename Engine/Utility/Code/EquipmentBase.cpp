#include "..\..\Header\EquipmentBase.h"
#include "Export_Function.h"

#include "Calculator.h"
#include "EquipInfo.h"
#include "Texture.h"
#include "RcTex.h"
#include "Animation.h"
#include "Transform.h"

#include "Export_Utility.h"

USING(Engine)

CEquipmentBase::CEquipmentBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CEquipmentBase::CEquipmentBase(const CEquipmentBase & rhs)
	: CGameObject(rhs)
{
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

	return S_OK;
}

void CEquipmentBase::Free(void)
{
	CGameObject::Free();
}
