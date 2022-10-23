#pragma once
#include "PSystem.h"
class CRealSnowFall :
	public CPSystem
{
public:
	CRealSnowFall(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX* boundingBox, int numParticles);
	void resetParticle(Attribute* attribute);
	virtual ~CRealSnowFall();
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual void		update(_float fTimeDelta);
	virtual void		Render_Obejct(void);
public:
	static CRealSnowFall*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);


private:
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransform = nullptr;
};

