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
	

public:
	void				Set_MouseToInventory();

public:
	_bool				EquipIconPicking();
	_matrix				m_ProjMatrix;

private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;
	CAnimation*				m_pAnimationCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	

private:
	HRESULT				Add_Component(void);

	// ������ ������ �⺻ źâ��
	_uint				m_iShotgunMagazine = 6;
	
	float			m_fX = 0.f;
	float			m_fY = 0.f;
	float			m_fSizeX = 0.f;
	float			m_fSizeY = 0.f;
	
	_bool			m_bOnce = false;

public:
	static CShotGun*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iZ);
	virtual void		Free(void);
	
};

