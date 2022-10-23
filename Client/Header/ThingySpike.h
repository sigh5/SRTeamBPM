#pragma once
#include "GameObject.h"
namespace Engine
{
	class CTexture;
	class CAnimation;
	class CRcTex;
	class CTransform;
}
class CThingySpike :public CGameObject
{
private:
	explicit CThingySpike(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThingySpike();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, float Wait);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	float				Get_Distance(void) { return m_fToPlayerDistance; }
public:
	static CThingySpike*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float wait, float Posx = 0, float Posy = 0);
	virtual void		Free(void);

private:
	CTransform*	m_pTransform = nullptr;
	CTexture*	 m_pTextureCom = nullptr;
	CRcTex*		m_pBufferCom = nullptr;
	CAnimation*	m_pAnimationCom = nullptr;

	float		m_fWaitTime = 0, f;
	float		m_fWaitTimeCounter = 0.f;
	bool		m_bHitPlayer = false;
	float		m_fToPlayerDistance = 0.f;
	bool		m_bOneTime = false;
};

