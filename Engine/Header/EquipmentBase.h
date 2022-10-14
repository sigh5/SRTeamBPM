#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

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

	EquipInfo&	Get_EquipInfoRef() { return m_EquipInfo; }
	void		Set_WeaponType(WEAPON_TYPE eID) { m_EquipInfo.m_WeaponType = eID; }
	WEAPON_TYPE	Get_WeaponType(void) { return m_EquipInfo.m_WeaponType; }
	_bool		Get_RenderControl(void) { return m_bRenderControl; }
	void		Set_RenderControl(_bool bRender) { m_bRenderControl = bRender; }


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
	_bool					m_bRenderControl = false;

protected:
	wstring				m_LayerName = L"";
	wstring				m_RoomName = L"";

public:
	virtual void		Free(void);
};

END