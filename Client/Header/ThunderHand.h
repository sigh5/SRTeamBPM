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
class CSkillParticle;

class CThunderHand : public CUI_Base
{
private:
	explicit CThunderHand(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThunderHand();

public:
	HRESULT			Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	void				Set_Active(_bool bEvent) { m_bActive = bEvent; }
	
	const _bool			Player_BuySkill() { return m_bBuySkill; }
	void				Set_BuySkill(_bool bEvent) { m_bBuySkill = bEvent; }


private:
	_bool				m_bActive = false;
	_float				m_fActiveTimer = 0.f;
	_bool				m_bBuySkill = false;
private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;

	_vec3				m_vecScale;
	CSkillParticle*		m_pParicle = nullptr;
	_bool				m_bOnce = true;





public:
	static CThunderHand*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

};

