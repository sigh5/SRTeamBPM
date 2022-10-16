#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRcTex;
class CTexture;
class CAnimation;
class CCalculator;
class CDynamic_Transform;
class COrthoTransform;

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
	HRESULT	 Ready_EquipInfo(_uint iPlusAtk, _uint iPlusDefense, _float fPlusSpeed, _float fPlusRange, WEAPON_TYPE eID);

	EquipInfo&			Get_EquipInfoRef() { return m_EquipInfo; }
	void				Set_WeaponType(WEAPON_TYPE eID) { m_EquipInfo.m_WeaponType = eID; }
	WEAPON_TYPE			Get_WeaponType(void) { return m_EquipInfo.m_WeaponType; }
	const _bool&		Get_IsWorld(void) { return m_bIsWorld; }
	void				Set_IsWorld(_bool bRender) { m_bIsWorld = bRender; }

	const _bool&		Get_IsInventory(void) { return m_bIsInventory; }
	void				Set_IsInventory(_bool bRender) { m_bIsInventory = bRender; }


public:
	void				Set_Layer_Map_Name(const wstring& LayerName, const wstring& RoomName)
	{
		m_LayerName = LayerName;
		m_RoomName = RoomName;
	}
protected:
	CAnimation*			m_pAnimationCom = nullptr;
	CDynamic_Transform* m_pDynamicTransCom = nullptr;
	COrthoTransform*	m_pOrthoTransCom = nullptr;
protected:
	virtual HRESULT				Add_Component(void);

protected:
	EquipInfo				m_EquipInfo;
	RENDERID				m_RenderID = RENDER_END;
	
	_bool					m_bIsWorld = true;
	_bool					m_bIsInventory = false;
	_bool					m_bIsPick = false;
protected:
	wstring				m_LayerName = L"";
	wstring				m_RoomName = L"";

	RECT				rcUI;

public:
	virtual void		Free(void);
};

END