#pragma once
#include "EquipmentBase.h"
BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CAnimation;
class CCalculator;
class CCollider;
END

class CInventory_UI;
class CEquipYeti :public CEquipmentBase
{
private:
	explicit CEquipYeti(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEquipYeti();

public:
	HRESULT				Ready_Object(_uint iX, _uint iZ);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void);
	virtual void		Collision_Event()override;
	virtual void		Change_Equip()override;

public:
	virtual void		Set_MouseToInventory();

public:
	void				PickingMouseUp();
	void				SearchInventorySlot(CInventory_UI** pInven);
	_bool				EquipIconPicking();
	void				Shop_Goods();

private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;

	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

private:
	HRESULT				Add_Component(void);


	_matrix				m_ProjMatrix;
	_bool			m_bOnce = false;
public:
	static CEquipYeti*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ);
	virtual void		Free(void);
};
