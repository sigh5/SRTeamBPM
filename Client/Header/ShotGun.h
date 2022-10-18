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

class CShotGun : public CEquipmentBase
{
private:
	explicit CShotGun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotGun();

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




private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;


private:
	HRESULT				Add_Component(void);

private:
	// 샷건이 가지는 기본 탄창량
	_uint				m_iShotgunMagazine = 6;

	_matrix				m_ProjMatrix;


	_bool			m_bOnce = false;

	_float			m_fTimedelta = 0.f;


	/*_bool			m_bPickingEnd = false;
	_bool			m_iMouseUpEnd = false;
	_bool			m_bisPicking = false;

	_int			m_iInvenSlotIndex = 0;*/

public:
	static CShotGun*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ);
	virtual void		Free(void);

};

