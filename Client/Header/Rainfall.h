#pragma once
#include "Particle.h"


class CRainfall : public CParticle
{
private:
	explicit CRainfall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRainfall();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	void Post_Render(void);
	void resetParticle(ParticleInfo* ParticleInfo);
	void Add_Texture();
	void UpdateRainfall(ParticleInfo* ParticleInfo) { ParticleInfo->vPos.y += ParticleInfo->vVelocity.y; }

public:
	static CRainfall*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

