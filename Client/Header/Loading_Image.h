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

class CLoading_Image : public Engine::CUI_Base
{
private:
	explicit CLoading_Image(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading_Image();

public:
	HRESULT Ready_Object(_bool _bTrue);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

	void	Set_B(_bool _b) { m_bB = _b; }

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CStart_Button*		m_pStartButton = nullptr;


	_bool				m_bB = false;
	_uint				m_iA = 0;
public:
	static CLoading_Image*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _bTrue);

private:

	virtual void Free(void);
};

