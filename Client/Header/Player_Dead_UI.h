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
	HRESULT			Ready_Object(_uint _iAlpha);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

	_uint			Get_iAlpha(void) { return m_iAlpha; }
	void			Set_iAlpha(_uint _iAlpha) { m_iAlpha = _iAlpha; }

	_bool			Get_Render(void) { return m_bRender; }
	void			Set_Render(_bool _bRender) { m_bRender = _bRender; }


	void			Set_bEvent(_bool _bEvent) { m_bEvent = _bEvent; }


private:
	HRESULT				Add_Component(void);

	HRESULT				Create_Quiz(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;

private:
	_vec3				m_vecScale;
	_uint				m_iAlpha = 0;
	// Fade-Out
	_bool				m_bRender = false;
	_bool				m_bEvent = false;

	// Fade-In
	_bool				m_bRenderIn = false;
	_bool				m_bPlusCount = false;

	_bool				m_bBGM = false;

	_bool				m_bQuiz = false; // Render_Font off

public:
	void			Set_RenderIn(_bool _bRenderIn) { m_bRenderIn = _bRenderIn; }
	void			Set_BGM(_bool _bBGM) { m_bBGM = _bBGM; }
	void			Set_Quiz(_bool _bQuiz) { m_bQuiz = _bQuiz; }
public:
	static CPlayer_Dead_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint _iAlpha);
	virtual void				Free();
};

