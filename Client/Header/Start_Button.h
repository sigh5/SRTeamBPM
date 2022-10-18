#pragma once
#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CRcTex;

class CStart_Button : public CGameObject
{
private:
	explicit CStart_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStart_Button();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

	_bool		MouseCheck(void);

public:
	_bool			Get_Click(void) { return m_bClick; }

	void			Set_Click(_bool _bClick) { m_bClick = _bClick; }

private:
	HRESULT			Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;

private:
	_bool			m_bCheck = false;
	_bool			m_bClick = false;

public:
	static CStart_Button*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY);

private:
	virtual void Free(void);
};

END