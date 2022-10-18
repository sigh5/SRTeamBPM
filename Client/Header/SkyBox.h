#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;

END
class CSkyBox :	public CGameObject
{
public:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBox();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
public:
	void				Set_ControlCubeCheck(_bool bEvent){m_bConrolTeleCubeCheck = bEvent;}
	const _bool&		Get_controlCubeCheck() { return m_bConrolTeleCubeCheck; }

private:
	CCubeTex*		m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	_vec3				m_vDirection;

private:
	// TeleCube Control
	_bool			m_bConrolTeleCubeCheck = false;


public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

