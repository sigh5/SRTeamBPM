#pragma once
#include "ItemBase.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CAnimation;
class CCollider;

END

class CCoin : public CItemBase
{
private:
	explicit CCoin(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCoin();

public:
	HRESULT				Ready_Object(_uint iX, _uint iY);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

	virtual void		Collision_Event()override;
	_bool				Get_bAddCoin() { return m_bAddCoin; }

private:
	HRESULT				Add_Component(void);

	void				Set_OnTerrain(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	// _vec3
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;

	_bool				m_bAddCoin = false;

public:
	static CCoin*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY);
	virtual void				Free(void);
};

