#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRcTex;
class CTexture;
class CAnimation;
class CCalculator;
class CDynamic_Transform;

class ENGINE_DLL CEquipmentBase : public CGameObject
{
protected:
	explicit CEquipmentBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEquipmentBase(const CEquipmentBase& rhs);
	virtual ~CEquipmentBase();


public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;
	HRESULT	 Ready_EquipInfo(_uint iPlusAtk, _uint iPlusDefense, _float fPlusSpeed, _float fPlusRange);

	EquipInfo&	Get_EquipInfoRef() { return m_EquipInfo; }
	void		Set_WeaponType(WEAPON_TYPE eID) { m_EquipInfo.m_WeaponType = eID; }
	WEAPON_TYPE	Get_WeaponType(void) { return m_EquipInfo.m_WeaponType; }

protected:
	CAnimation*			m_pAnimationCom = nullptr;
	CDynamic_Transform* m_pDynamicTransCom = nullptr;

protected:
	virtual HRESULT				Add_Component(void);

protected:
	EquipInfo			 m_EquipInfo;

public:
	virtual void		Free(void);
};

END