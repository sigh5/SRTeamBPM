#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;
class CCollider;
END


class CTeleCube :public CGameObject
{
private:
	explicit CTeleCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CTeleCube();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event() override;

public:
	const _int&			Get_Option() { return m_iOption; }
	void				Set_Option(CUBE_TYPE eType) { m_iOption = (_int)eType; }

	void				Random_ResurrectionRoom();		// 나중에 플레이어한테 붙일거임


private:
	HRESULT				Add_Component(void);

public:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	_int				m_iOption = 0;					// 0: Wall 1: Obstacle 2: TeleportStart 3: TelePortEnd


public:
	static CTeleCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free()override;

};

