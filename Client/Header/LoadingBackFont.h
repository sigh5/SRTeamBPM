#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CLoadingBackFont : public CUI_Base
{
private:
	explicit CLoadingBackFont(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLoadingBackFont(const CUI_Base& rhs);
	virtual ~CLoadingBackFont();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

private:
	_vec3				m_vecScale;
	_uint				m_iRandomText = 0;

public:
	static CLoadingBackFont*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY);
	virtual void					Free();
};

