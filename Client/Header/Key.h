#pragma once
#include "ItemBase.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

class CCollider;

END

class CKey : public CItemBase
{
private:
	explicit CKey(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKey();

public:
	HRESULT				Ready_Object(_uint iX, _uint iY);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

	virtual void		Collision_Event()override;
	_bool				Get_bAddKey() { return m_bAddKey; }

private:
	HRESULT				Add_Component(void);

	void				Set_OnTerrain(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	
	CCollider*			m_pColliderCom = nullptr;

private:
	_bool				m_bAddKey = false;

public:
	static CKey*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY);
	virtual void				Free(void);
};

