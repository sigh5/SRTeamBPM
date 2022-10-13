#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CTexture;
class CCalculator;
class COrthoTransform;
class CRcTex;
class CAnimation;

END

class CCoinKeyUI : public CUI_Base
{
public:
	CCoinKeyUI(LPDIRECT3DDEVICE9 pGraphicDev);
	CCoinKeyUI(const CUI_Base& rhs);
	virtual ~CCoinKeyUI();

public:
	HRESULT				Ready_Object(CGameObject* pPlayer);
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

	CGameObject*		m_pPlayer;

private: // Status 각 요소에 쓰는 폰트
	wstring				m_szCoin = L"";		  // The number of coins the player has
	wstring				m_szKey = L"";	  // The number of Keys the player has

	_uint				m_iPlayerCoin = 0;
	_uint				m_iPlayerKey = 0;
	
	_vec3				m_vecScale;
public:
	static CCoinKeyUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void			Free();

};

