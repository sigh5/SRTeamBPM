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

	void	Picking(void);
	
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

	// ������ ������ �⺻ źâ��
	_uint				m_iShotgunMagazine = 6;

	_bool				m_bRenderFalse = false;

	_matrix				m_matProj;

	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;


public:
	static CShotGun*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ);
	virtual void		Free(void);
	
};

