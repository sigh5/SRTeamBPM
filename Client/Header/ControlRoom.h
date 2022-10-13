#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CSphereTex;
class CTransform;
class CCalculator;
class CCollider;

END


class CControlRoom :public CGameObject
{
private:
	explicit CControlRoom(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CControlRoom();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event() override;

public:
	void				Set_Active(_bool bActive) { m_bSetActive = bActive; }
	const _bool&		Get_Active() { return m_bSetActive; }

private:
	HRESULT				Add_Component(void);

public:
	CSphereTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	
private:
	_bool				m_bSetActive = false;

public:
	static CControlRoom* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		 Free()override;

};

