#pragma once
#include "BaseBullet.h"
BEGIN(Engine)

class CTransform;
class CTexture;
class CCubeTex;
class CCollider;
class CRcTex;
END
class CPiercingBullet :public CBaseBullet
{
private:
	explicit CPiercingBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPiercingBullet();

public:
	HRESULT				Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

	void				Set_Bullet_Dir(_int iDir);

private:
	HRESULT				Add_Component(void);

private:
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	//CCubeTex*			m_pCubeTexCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	_float				m_fBulletSpeed = 25.f;

	_vec3				m_vRight;

	_int				m_iDir = 0;


public:
	static CPiercingBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
};

