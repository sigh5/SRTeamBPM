#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CRcTex;

class CExit_Button : public CGameObject
{
private:
	explicit CExit_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExit_Button();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

	_bool		MouseCheck(void);

public:
	_bool			Get_Click(void) { return m_bClick; }

private:
	HRESULT			Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;

private:
	_bool			m_bCheck = FALSE;
	_bool			m_bClick = FALSE;


public:
	static CExit_Button*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY);

private:
	virtual void Free(void);

};

END