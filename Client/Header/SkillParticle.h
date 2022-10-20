#pragma once
#include "PSystem.h"
class CSkillParticle :public CPSystem
{
private:
	CSkillParticle(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX* boundingBox, int numParticles, _vec3 vPos);
	void resetParticle(Attribute* attribute);
	virtual ~CSkillParticle();
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual void		update(_float fTimeDelta);
	virtual void		Render_Obejct(void);

public:
	static CSkillParticle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void);

private:
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransform = nullptr;

};

