#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRcTex;
END
class CBulletShell :public CGameObject
{
private:
	explicit CBulletShell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBulletShell();

public:
	HRESULT				Ready_Object(_vec3 vPos, _vec3 vDir, int shelltype = 0);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;

	_vec3			m_vDir; //Player Right
	_vec3			m_vUpDown;
	int				m_iDropGround = 0;
	float			m_fSpin = 0.f;
	float			m_fSpinTime = 0.f;
	float			m_fSpinTimeCount = 0.f;
	int				m_iShellType = 0;

	
public:
	static CBulletShell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRight, int shelltype = 0);
	virtual void		Free(void);
};

