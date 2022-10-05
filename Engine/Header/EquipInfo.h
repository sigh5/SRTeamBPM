#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CEquipInfo : public CComponent
{
private:
	explicit CEquipInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEquipInfo(const CEquipInfo& rhs);
	virtual ~CEquipInfo();

public:
	HRESULT	 Ready_EquipInfo(_uint iPlusAtk, _uint iPlusDefense, _float fPlusSpeed, _float fPlusRange);

	EquipInfo&	Get_EquipInfoRef() { return m_EquipInfo; }

private:
	EquipInfo		 m_EquipInfo;
	bool			 m_bClone;

public:

	virtual CComponent*			Clone(void) override;
	static CEquipInfo*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iPlusAtk, _uint iPlusDefense, _float fPlusSpeed, _float fPlusRange);

private:
	virtual void				Free(void) override;
};

END