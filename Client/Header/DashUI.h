#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"
#include "TestPlayer.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class COrthoTransform;
class CCalculator;
class CAnimation;

END

class CDashUI : public CUI_Base
{
public:
	 explicit CDashUI(LPDIRECT3DDEVICE9 pGraphicDev);
	 explicit CDashUI(const CUI_Base& rhs);
	virtual ~CDashUI();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	//CGameObject*		m_pPlayer;

private:
	_vec3				m_vecScale;
	
	_float				m_fDelay = 0.f;
	_float				m_fDelayTime = 0.f;

	_bool				m_bTest = false;

public:
	static CDashUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free();

};

