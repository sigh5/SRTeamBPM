#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class COrthoTransform;

END

class CPlayer_Dead_UI : public Engine::CUI_Base
{
private:
	explicit CPlayer_Dead_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer_Dead_UI();

public:
	HRESULT			Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

	_uint			Get_iAlpha(void) { return m_iAlpha; }

private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;

private:
	_vec3				m_vecScale;
	_uint				m_iAlpha = 255;

public:
	static CPlayer_Dead_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free();
};

