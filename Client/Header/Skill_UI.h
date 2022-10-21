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

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;

private:
	_vec3				m_vecScale;

public:
	static CSkill_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free();
};

