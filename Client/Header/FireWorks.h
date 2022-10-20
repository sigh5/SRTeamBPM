#pragma once
#include "PSystem.h"
BEGIN(Engine)
class CAnimation;
END

class CFireWorks :public CPSystem
{
private:
	CFireWorks(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX* boundingBox, int numParticles, _vec3 vPos);
	void resetParticle(Attribute* attribute);
	virtual ~CFireWorks();
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual void		update(_float fTimeDelta);
	virtual void		Render_Obejct(void);
	virtual void		preRender();
	virtual void		postRender();

public:
	static CFireWorks*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void);

private:
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransform = nullptr;
	CAnimation* m_pAnimationCom = nullptr;
};

