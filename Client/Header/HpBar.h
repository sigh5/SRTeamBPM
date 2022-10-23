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



class CHpBar : public CUI_Base
{
public:
	explicit CHpBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHpBar(const CUI_Base& rhs);
	virtual ~CHpBar();

public:
	HRESULT				Ready_Object(CGameObject* pPlayer);
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;
	
private:
	HRESULT				Add_Component(void);
	//void				Render_PlayerHpFont(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CGameObject*		m_pPlayer;
	
private:
	_vec3				m_vecScale;

	// Player's Hp Font
	_uint				m_iPlayerHp;

	_uint				m_iHpFont;
	
	wstring				m_szPlayerHp = L"";


public:
	static CHpBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void Free();
};

