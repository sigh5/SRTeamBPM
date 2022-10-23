#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimation;

END

class CThunderPic : public CUI_Base
{
private:
	explicit CThunderPic(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CThunderPic(const CUI_Base& rhs);
	virtual ~CThunderPic();

public:
	virtual HRESULT				Ready_Object() override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				LateUpdate_Object(void) override;
	virtual void				Render_Obejct(void) override;

	void						Set_SoldOut(_bool _bSoldOut) { m_bSoldOut = _bSoldOut; }
	void						Set_PicRender(_bool _bRender) { m_bRender = _bRender; }
	void						Set_Exit(_bool _bExit) { m_bExit = _bExit; }

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

	_bool						m_bExit = false;

	_vec3						m_vecScale;

public:
	static CThunderPic*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void			Free(void);
};

