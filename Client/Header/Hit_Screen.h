#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CShader;

END

class CHit_Screen : public CUI_Base
{
private:
	explicit CHit_Screen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHit_Screen(const CUI_Base& rhs);
	virtual ~CHit_Screen();

public:
	virtual HRESULT				Ready_Object() override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				LateUpdate_Object(void) override;
	virtual void				Render_Obejct(void) override;

	void						Set_Render(_bool _bRender) { m_bRender = _bRender; }
	_bool						Get_Render(void) { return m_bRender; }

private:
	HRESULT						Add_Component(void);

private:
	CRcTex*						m_pBufferCom = nullptr;
	CTransform*					m_pTransCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CShader*					m_pShaderCom = nullptr;

private:
	_vec3						m_vecScale;
	_bool						m_bRender = false;

	_float						m_fTime = 0.f;

public:
	static CHit_Screen*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void				Free(void);
};

