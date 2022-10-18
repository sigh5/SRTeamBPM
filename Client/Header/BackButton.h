#pragma once
#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CRcTex;

END

class CBackButton : public CGameObject
{
private:
	explicit CBackButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackButton();

public:
	virtual HRESULT			Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void) override;
	virtual void			Render_Obejct(void) override;

	_bool					MouseCheck(void);

public:
	_bool					Get_Click(void) { return m_bClick; }

	void					Set_Click(_bool _bClick) { m_bClick = _bClick; }
private:
	HRESULT					Add_Component(void);

private:
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransCom = nullptr;
	CRcTex*					m_pBufferCom = nullptr;

private:	
	_bool					m_bClick = false;

	RECT					rcButton; 

public:
	static CBackButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY);

private:
	virtual void			Free(void);
};

