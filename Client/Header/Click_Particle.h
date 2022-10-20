#pragma once
#include "PSystem.h"
class CClick_Particle : public CPSystem
{
private:
	CClick_Particle(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX* boundingBox, _uint numParticles, _vec3 vPos);

	virtual ~CClick_Particle();

	void resetParticle(Attribute* attribute);

	virtual	_int		Update_Object(const _float& fTimeDelta);
	virtual void		update(_float fTimeDelta);
	virtual void		Render_Obejct(void);
	virtual void		preRender();
	virtual void		postRender();

public:
	static CClick_Particle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void				Free(void);

private:
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransform = nullptr;

};

