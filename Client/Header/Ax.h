#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCalculator;
class CAnimation;
class COrthoTransform;

END

class CAx : public CUI_Base
{
public:
	explicit CAx(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAx(const CUI_Base& rhs);
	virtual ~CAx();

public:
	HRESULT			Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	void				Set_Active(_bool bEvent) { m_bActive = bEvent; }
	void				Set_GunNoRender(_bool bEvent) { m_bNoRender = bEvent; }
	void				Set_ChangeWeaponUI(_bool bChangeWeaponUI) { m_bChangeWeaponUI = bChangeWeaponUI; }

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;

private:
	_bool				m_bActive = false;
	_bool				m_bNoRender = false;
	_bool				m_bAnimation = false;
	_bool				m_bChangeWeaponUI = false;
	_float				m_fActiveTimer = 0.f;
	
	_vec3				m_vecScale;

public:
	static CAx*				Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

};

