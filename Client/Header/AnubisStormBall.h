#pragma once
#include "BaseBullet.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CAnimation;
class CRcTex;

END

class CAnubisStormBall :
	public CBaseBullet
{
private:
	explicit CAnubisStormBall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubisStormBall();

public:
	HRESULT				Ready_Object(_vec3 vPos, _vec3 vTarget);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();

private:
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	//CCubeTex*			m_pCubeTexCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

public:
	static CAnubisStormBall*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTarget);
	virtual void		Free(void);

	float			m_fLifetime = 0.f;
	float			m_fLifetimeCount = 0.f;

	bool			m_bHitPlayer = false;
};

