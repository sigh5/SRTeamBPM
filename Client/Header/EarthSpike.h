#pragma once
#include "GameObject.h"
namespace Engine
{
	class CTexture;
	class CAnimation;
	class CRcTex;
	class CTransform;
}

class CEarthSpike :
	public CGameObject
{
public:
	explicit CEarthSpike(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEarthSpike();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, bool spiketype, float Wait);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

public:
	static CEarthSpike*		Create(LPDIRECT3DDEVICE9 pGraphicDev,float wait, float Posx = 0, float Posy = 0, bool spiketype = false);
	virtual void		Free(void);

private:
	CTransform*	m_pTransform = nullptr;
	CTexture*	 m_pTextureComOne = nullptr;
	CTexture*	m_pTextureComTwo = nullptr;
	CRcTex*		m_pBufferCom = nullptr;
	CAnimation*	m_pAnimationCom = nullptr;

	bool		m_bSpikeType = false;
	float		m_fWaitTime = 0,f;
	float		m_fWaitTimeCounter = 0.f;
	bool		m_bHitPlayer = false;
};

