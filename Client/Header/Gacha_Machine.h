#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CAnimation;
class CCollider;

END

class CGacha_Machine : public CGameObject
{
private:
	explicit CGacha_Machine(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGacha_Machine();

public:
	HRESULT				Ready_Object(_uint iX, _uint iY);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

	HRESULT				Open_Event(CGameObject * pPlayer);

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	_bool				m_bTextRender = false;

public:
	static CGacha_Machine*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY);
	virtual void				Free(void);
};

