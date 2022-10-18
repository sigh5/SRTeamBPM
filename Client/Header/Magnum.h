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

class CMagnum : public CEquipmentBase
{
public:
	explicit CMagnum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMagnum();

public:
	virtual HRESULT			Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta);
	virtual void			LateUpdate_Object(void);
	virtual void			Render_Obejct(void);
	virtual void			Collision_Event()override;
	virtual void			Change_Equip()override;

public:
	virtual void			Set_MouseToInventory();

public:
	void					PickingMouseUp();
	void					SearchInventorySlot(CInventory_UI** pInven);
	_bool					EquipIconPicking();




private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;


private:
	HRESULT					Add_Component(void);




private:
	// 매그넘이 가지는 기본 탄창량
	_uint					m_iMagnumMagazine = 8;

	_bool			m_bOnce = true;		// 처음에 기본무기 세팅하기위해서
	_float			m_fTimedelta = 0.f;
	
	_matrix			m_ProjMatrix;
	
	



public:
	static CMagnum*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

