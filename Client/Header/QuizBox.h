#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CQuizBox : public CUI_Base
{
private:
	explicit CQuizBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CQuizBox(const CUI_Base& rhs);
	virtual ~CQuizBox();

public:
	HRESULT Ready_Object(_bool _bRenderQuiz);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

	void Quiz_Answer(void);

private:
	HRESULT		Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	// Control
	_uint				m_iRandom = 0;
	_uint				m_iCount = 0;

	_bool				m_bFalseAnswer = false;
	_bool				m_bRenderQuiz = false;
	_bool				m_bTest = false;
	_bool				m_bRandom = false;
	// ~Control

public:
	static CQuizBox*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _bRenderQuiz);

private:
	virtual		void			Free(void);
};

