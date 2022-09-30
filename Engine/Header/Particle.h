#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;

class ENGINE_DLL CParticle :
	public CGameObject
{
protected:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CParticle();

public:
	virtual		bool Ready_Particle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		_int Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Obejct(void);

	virtual		void reset();
	virtual		void resetParticle(ParticleInfo* ParticleInfo) PURE;
	virtual		void addParticle();


public:
	bool isEmpty();
	bool isDead();

protected:
	virtual void removeDeadParticleds();
	//collect

protected:
	_vec3		m_Origin;
	_vec3		m_BoundingLeftBottom;
	_vec3		m_BoundingRightTop;
	_float		m_EmitRate;
	_float		m_Size;

	CTexture* m_TextureCom;

	LPDIRECT3DVERTEXBUFFER9 m_vb;
	//list<ParticleInfo>	m_Particlelist;
	vector<ParticleInfo>	m_vecParticles;
	_int		m_MaxParticles;

	DWORD m_vbSize;
	DWORD m_vbOffSet;
	DWORD m_vbBatchSize;
};

END