#pragma once

#include "UI_Base.h"
#include "Engine_Include.h"
namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CAnimation;
	class COrthoTransform;
}


class CUI_Effect :public CUI_Base
{
private:
	explicit CUI_Effect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI_Effect();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;


public:
	void				Set_Active(_bool bEvent) { m_bActive = bEvent;}
	const _bool			Get_Active() { return m_bActive; }

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

private:
	_vec3				m_vecScale;

	_float				m_fDelay = 0.f;
	_float				m_fDelayTime = 0.f;

	_bool				m_bSize = false;
	_bool				m_bActive = false;


public:
	static CUI_Effect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = _vec3{ 0.f,0.f,0.f });
	virtual void			Free(void);



};

