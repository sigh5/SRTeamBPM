#pragma once
#include "EquipmentBase.h"
#include "TestPlayer.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRcTex;
class CAnimation;
class CCalculator;
class CCollider;
END

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


private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	
public:
	void				Set_MouseToInventory();
	_bool				EquipIconPicking();
	
private:
	HRESULT					Add_Component(void);

private:
	// 매그넘이 가지는 기본 탄창량
	_uint					m_iMagnumMagazine = 8;
	_float			m_fX = 0.f;
	_float			m_fY = 0.f;
	_float			m_fSizeX = 0.f;
	_float			m_fSizeY = 0.f;
	_bool			m_bOnce = false;
	_float			m_fTimedelta = 0.f;
	
	_matrix			m_ProjMatrix;

	_bool			m_Picking_End = false;

public:
	static CMagnum*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

