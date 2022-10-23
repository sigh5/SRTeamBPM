#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CTexture;
class COrthoTransform;
class CRcTex;
class CAnimation;

END

class CDefBar : public CUI_Base
{
private:
	explicit CDefBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDefBar(const CUI_Base& rhs);
	virtual ~CDefBar();

public:
	virtual	HRESULT				Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

private:
	_vec3				m_vecScale;

	// Player's Def
	_uint				m_iPlayerDef;

public:
	static CDefBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

