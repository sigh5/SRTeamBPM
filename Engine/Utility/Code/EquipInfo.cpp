#include "..\..\Header\EquipInfo.h"
#include "Export_Function.h"

USING(Engine)

CEquipInfo::CEquipInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_bClone(false)
{
}

CEquipInfo::CEquipInfo(const CEquipInfo & rhs)
	: CComponent(rhs), m_bClone(true)
{
}

CEquipInfo::~CEquipInfo()
{
}

HRESULT CEquipInfo::Ready_EquipInfo(_uint iPlusAtk, _uint iPlusDefense, _float fPlusSpeed, _float fPlusRange)
{
	memcpy(&m_EquipInfo._iAddAttack, &iPlusAtk, sizeof(_uint));
	memcpy(&m_EquipInfo._iAddDefense, &iPlusDefense, sizeof(_uint));
	memcpy(&m_EquipInfo._fAddSpeed, &fPlusSpeed, sizeof(_float));
	memcpy(&m_EquipInfo._fAddRange, &fPlusRange, sizeof(_float));

	return S_OK;
}

CComponent * CEquipInfo::Clone(void)
{
	return new CEquipInfo(*this);
}

CEquipInfo * CEquipInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iPlusAtk, _uint iPlusDefense, _float fPlusSpeed, _float fPlusRange)
{
	CEquipInfo* pInstance = new CEquipInfo(pGraphicDev);

	if (FAILED(pInstance->Ready_EquipInfo(iPlusAtk, iPlusDefense, fPlusSpeed, fPlusRange)))
	{
		MSG_BOX("EquipInfo Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CEquipInfo::Free(void)
{
	CComponent::Free();
		
}
