#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class COrthoTransform;

END

class CSkill_UI : public CUI_Base
{
private:
	explicit CSkill_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkill_UI(const CUI_Base& rhs);
	virtual ~CSkill_UI();

public:
	virtual HRESULT		Ready_Object() override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

	void				Set_m_bRB(_bool _bRButton) { m_bRB = _bRButton; }

	void				Set_mbRshift(_bool _bRBtn) { m_bRshift = _bRBtn; }

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;

private:
	_vec3				m_vecScale;

	_bool				m_bRB = false;

	_bool				m_bSize = false;
	_bool				m_bRshift = false;

	_float				m_fDelayTime = 0.f;
	_float				m_fDelay = 0.f;

public:
	static CSkill_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free();
};

