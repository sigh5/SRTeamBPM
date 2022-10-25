#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END
class CMsgUI :public CUI_Base
{
private:
	explicit CMsgUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMsgUI();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

public:
	void				Set_FontMsg(wstring pStr, _float PosX, _float PosY);
	void				Set_MsgFrame(_float fx, _float fy, _float fSizeX, _float fSizxY);

	void				Set_Active(_bool bEvent) { m_bSetActive = bEvent; }

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	_matrix				m_ProjMatrix;

	
	_bool				m_bSetActive = false;
	_float				m_bActiveTimer = 0.f;


	_float				m_fSetWidth = 0.f;
	_float				m_fSetHeight = 0.f;
private:
	_vec3				m_vecScale;
	_uint				m_iRandomText = 0;
	wstring				pString = L"";
public:
	static CMsgUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void					Free();
};

