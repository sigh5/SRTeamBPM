#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CDynamic_Transform;
class CRcTex;
class CTexture;
class CCalculator;
class CAnimation;
class COrthoTransform;

END

class CGun_Screen : public CGameObject
{
public:
	explicit CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGun_Screen(const CGun_Screen& rhs);
	virtual ~CGun_Screen();

public:
	HRESULT			Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	HRESULT				Shoot_Motion(void);

public:	// Get/Set
	const _bool&		Get_ReadyShot(void)const		 { return m_bReadyShot; }
	void				Set_ReadyShot(_bool bMetro)		{ m_bReadyShot = bMetro; }

	const _uint&		Get_Magazine(void)const		 { return m_iMagazine; }
	void				Set_Magazine(_int iNumber)	 { m_iMagazine = iNumber; }
	void				Add_Magazine(_int iNumber)	 { m_iMagazine += iNumber; }

	void				Set_AnimationCheck(_bool bAnimation) { m_bAnimation = bAnimation; }
	
	void				Set_Shoot(_bool bShoot) { m_bShootCheck = bShoot; }
	const _bool &		Get_Shoot()const { return m_bShootCheck; }

public:
	void				GunFailSound();

private:
	_bool				m_bShootCheck = false;
	_bool				m_bReadyShot = false;
	_uint				m_iMagazine = 8;
	_bool				m_bAnimation = false;

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	COrthoTransform*	m_pOrthoTransCom = nullptr;

public:
	static CGun_Screen*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

};

