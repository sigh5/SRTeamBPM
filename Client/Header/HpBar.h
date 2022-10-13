#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"
#include "TestPlayer.h"

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

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CGameObject*		m_pPlayer;
	
	_vec3				m_vecScale;

public:
	static CHpBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual void Free();
};

