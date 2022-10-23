#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimation;

END

class CHelmetPic : public CUI_Base
{
private:
	explicit CHelmetPic(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHelmetPic(const CUI_Base& rhs);
	virtual ~CHelmetPic();

public:
	virtual HRESULT				Ready_Object() override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				LateUpdate_Object(void) override;
	virtual void				Render_Obejct(void) override;

	void						Set_SoldOut(_bool _bSoldOut) { m_bSoldOut = _bSoldOut; }
	void						Set_PicRender(_bool _bRender) { m_bRender = _bRender; }

private:
	HRESULT						Add_Component(void);

private:
	CRcTex*						m_pBufferCom = nullptr;
	CTransform*					m_pTransCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CAnimation*					m_pAnimationCom = nullptr;

private:
	_bool						m_bSoldOut = true;

	_bool						m_bRender = false;

	_vec3						m_vecScale;

public:
	static CHelmetPic*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void			Free(void);
};

