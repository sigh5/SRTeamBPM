#pragma once
#include "GameObject.h"
#include "Base_Effect.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CAnimation;
}

class CHitBlood : public CGameObject
{
private:
	explicit CHitBlood(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHitBlood();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

public:
	_float				m_fFrame = 0.f;


public:
	static CHitBlood*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = _vec3{ 0.f,0.f,0.f });
	virtual void	Free(void);
};


