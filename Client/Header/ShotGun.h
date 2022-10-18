#pragma once
#include "EquipmentBase.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CAnimation;
class CCalculator;
class CCollider;

END

class CShotGun : public CEquipmentBase
{
private:
	explicit CShotGun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotGun();

public:
	HRESULT				Ready_Object(_uint iX, _uint iZ);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;
	
	void				 Set_OnTerrain(void);

	_bool				Get_RenderFalse(void) { return m_bRenderFalse; }

	_bool	Picking(void);

	_bool				Get_bPicking(void) { return m_bPick; }
	
private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	
private:
	HRESULT				Add_Component(void);

public:
	HRESULT				Open_Event(CGameObject* pGameObject);

	// 샷건이 가지는 기본 탄창량
	_uint				m_iShotgunMagazine = 6;

	_bool				m_bRenderFalse = false;

	_matrix				m_matProj;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	// for Inventory
	_uint iIndexRow = 0;
	_uint iIndexColumn = 0;

	_uint iPickRow = 0;
	_uint iPickColumn = 0;

	_bool	m_bPick = false;

	RECT		rcUI;
	// ~for Inventory

public:
	static CShotGun*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ);
	virtual void		Free(void);
	
};

