#pragma once
#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CRcTex;

class CMyButton : public Engine::CGameObject
{
private:
	explicit CMyButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMyButton();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

	_bool		MouseCheck(void);


private:
	HRESULT			Add_Component(void);

private:
	RECT			m_tRect;

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransCom = nullptr;


public:
	static CMyButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

END