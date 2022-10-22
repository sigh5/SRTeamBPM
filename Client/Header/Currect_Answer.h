#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CAnimation;

END

class CCurrect_Answer : public CUI_Base
{
private:
	explicit CCurrect_Answer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCurrect_Answer(const CUI_Base& rhs);
	virtual ~CCurrect_Answer();

public:
	HRESULT						Ready_Object(_float fX, _float fY);
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				LateUpdate_Object(void) override;
	virtual void				Render_Obejct(void) override;

private:
	HRESULT						Add_Component(void);

private:
	CRcTex*						m_pBufferCom = nullptr;
	CTransform*					m_pTransCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CAnimation*					m_pAnimationCom = nullptr;

private:
	_bool						m_bRender = false;

	_float ffx;
	_float ffy;

public:
	static CCurrect_Answer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY);

private:
	virtual void				Free(void);
};

