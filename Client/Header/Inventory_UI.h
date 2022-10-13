#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class COrthoTransform;
class CCalculator;
class CAnimation;

class CCollider;

class CEquipmentBase;

END

class CInventory_UI : public CUI_Base
{
private:
	explicit CInventory_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory_UI(const CUI_Base& rhs);
	virtual ~CInventory_UI();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

	void				Find_Equip_Item(void);

	_bool				Get_InvenSwitch(void) { return m_bInvenSwitch; }

private:
	HRESULT				Add_Component(void);
	
public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CCollider*			m_pColliderCom = nullptr;

private:
	_vec3				m_vecScale;

	_bool				m_bInvenSwitch = false;

	vector<CEquipmentBase*>		m_vecWeaponType;
	
public:
	vector<CEquipmentBase*>*	Get_WeaponType(void) { return &m_vecWeaponType; }

public:
	static CInventory_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free();
};

