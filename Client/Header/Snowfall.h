#pragma once
#include "PSystem.h"
class CSnowfall :public CPSystem
{
public:
	CSnowfall(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX* boundingBox, int numParticles);
	void resetParticle(Attribute* attribute);
	virtual ~CSnowfall();
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual void		update(_float fTimeDelta);
	virtual void		Render_Obejct(void);
public:
	static CSnowfall*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);

	//unsigned long m_ResetCount;

private:
	CTexture* m_pTextureCom = nullptr;
};

