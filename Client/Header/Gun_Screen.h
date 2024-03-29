#pragma once
//#include "GameObject.h"
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCalculator;
class CAnimation;
class COrthoTransform;

END

class CGun_Screen : public CUI_Base
{
public:
	explicit CGun_Screen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGun_Screen(const CUI_Base& rhs);
	virtual ~CGun_Screen();

public:
	HRESULT			Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

	HRESULT				Add_UpdateComponent(void);

public:
	HRESULT				Shoot_Motion(const _float& fTimeDelta);

public:	// Get/Set
	const _bool&		Get_ReadyShot(void)const		 { return m_bReadyShot; }
	void				Set_ReadyShot(_bool bMetro)		{ m_bReadyShot = bMetro; }

	const _uint&		Get_Magazine(void)const		 { return m_iMagazine; }
	void				Set_Magazine(_int iNumber)	 { m_iMagazine = iNumber; } // 각 총 별 정보에서 탄창량도 제어할 것
	void				Add_Magazine(_int iNumber)	 { m_iMagazine += iNumber; }

	void				Set_AnimationCheck(_bool bAnimation) { m_bAnimation = bAnimation; }
	
	void				Set_Shoot(_bool bShoot) { m_bShootCheck = bShoot; }
	const _bool &		Get_Shoot()const { return m_bShootCheck; }


	void				Set_ChangeWeaponUI(_bool bChangeWeaponUI) { m_bChangeWeaponUI = bChangeWeaponUI; }

	void				Set_ChaneWeaponUI_ID(_bool bChangeWeaponUI, _uint iID)
	{
		m_bChangeWeaponUI = bChangeWeaponUI;
		
		if (iID != m_iID)
		{
			m_bControl = false;
		}

		m_iID = iID;
	}


public:
	void				GunFailSound();
	void				Set_Active(_bool bEvent) { m_bActive = bEvent; }

	void				Set_GunNoRender(_bool bEvent) { m_bNoRender = bEvent; }

	void				Get_shellPosition(_vec3& vPos, _vec3& vdir);

	_uint				Get_miID(void) { return m_iID; }

private:
	_bool				m_bShootCheck = false;
	_bool				m_bReadyShot = false;
	_uint				m_iMagazine = 8;
	_bool				m_bAnimation = false;


	_bool				m_bChangeWeaponUI = false;
	_bool				m_bControl = false;
	_uint				m_iAlpha = 0;
	_vec3				m_vecScale;

	_bool				m_bActive = false;
	_float				m_fActiveTimer = 0.f;
	
	_bool				m_bNoRender = false;

	bool				m_bCreatedShell = false;
	bool				m_bShotgunSound = false;

	_uint				m_iID = 0;		

private:
	CRcTex*				m_pBufferCom = nullptr;
	
	CTexture*			m_pTextureMainCom = nullptr;
	
	CTexture*			m_pTextureMagnumGun = nullptr;
	CTexture*			m_pTextureShotGun = nullptr;


	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;

public:
	static CGun_Screen*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

};

