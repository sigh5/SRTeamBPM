#pragma once
#include "BaseBullet.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CCubeTex;
class CAnimation;
class CRcTex;

END
class CFinalBossBullet :public CBaseBullet
{
private:
	explicit CFinalBossBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalBossBullet();

public:
	HRESULT				Ready_Object(_vec3 vPos, CGameObject* pMaster);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CTexture*			m_pDeadTextureCom = nullptr;
	bool				m_bHitPlayer = false;
	CAnimation*			m_pDeadAnimationCom = nullptr;
	bool				m_bExplosionSound = false;
	bool				m_bStopGo = false;
	float				m_fPlusSize = 0.f;
	bool				m_bChargeSound = false;

	CGameObject*		m_pMaster = nullptr;

public:
	static CFinalBossBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CGameObject* pMaster);
	virtual void		Free(void);
};

