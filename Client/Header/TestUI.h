#pragma once
#include "Engine_Include.h"
#include "UI_Base.h"

#include "Start_Button.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimation;

END

class CTestUI : public Engine::CUI_Base
{
private:
	explicit CTestUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestUI();

public:
	HRESULT Ready_Object(_bool _bTrue);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	//CStart_Button*		m_pStartButton = nullptr;


	_bool				m_bChangePNG = false;
	_uint				m_iChangePNG = 0;
public:
	static CTestUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _bTrue);

private:

	virtual void Free(void);
};

