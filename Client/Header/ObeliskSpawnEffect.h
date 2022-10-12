#pragma once
#include "GameObject.h"
namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CAnimation;
}

class CObeliskSpawnEffect :
	public CGameObject
{
private:
	explicit CObeliskSpawnEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObeliskSpawnEffect();

public:
	virtual HRESULT Ready_Object(const _vec3& vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

public:
	static CObeliskSpawnEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
	virtual void	Free(void);
};

