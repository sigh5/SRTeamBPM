#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CTexture;
class CThunderTex;
class CTransform;
class CRcTex;
class CAnimation;
END
class CFirePillar :
	public CGameObject
{
private:
	explicit CFirePillar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFirePillar();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, float Posz);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
public:
	static CFirePillar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Posx = 0, _float Posy = 0, _float Posz = 0);
	virtual void		Free(void);

private:
	CTexture*		 m_pTextureCom = nullptr;
	CTexture*		m_pReadyTextureCom = nullptr;
	CTransform*		m_pTransform = nullptr;
	CRcTex*			m_pReadyBufferCom = nullptr;
	CAnimation*		m_pReadyAnimationCom = nullptr;
	CAnimation*		m_pAnimationCom = nullptr;
	CThunderTex*	m_pBufferCom = nullptr;
	bool			m_bHitPlayer = false;

	float			m_fLifetime = 0.f;
	float			m_fLifetimeCount = 0.f;


	int				m_iState = 0;
	bool			m_bReadySound = false;
	bool			m_bFlameSound = false;
};

