#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;
class CCollider;
class CShader;
END

//주석지우셈
class CTeleCube :public CGameObject
{
private:
	explicit CTeleCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CTeleCube();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object();
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event() override;
	void				Set_ForceScene(_int iNum);
public:
	const _int&			Get_Option() { return m_iOption; }
	void				Set_Option(CUBE_TYPE eType) { m_iOption = (_int)eType; }

	void				Random_ResurrectionRoom();		// 나중에 플레이어한테 붙일거임

	void				Set_Active(_bool bActive) { m_bSetActive = bActive; }
	const _bool&		Get_Active() { return m_bSetActive; }
	
	
	_int			m_iForceSceneReturn = 0;


private:
	HRESULT				Add_Component(void);

public:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	_bool				m_bOnce = false;
	_int				m_iOption = 0;					// 0: Wall 1: Obstacle 2: TeleportStart 3: TelePortEnd
	_bool				m_bAlpha = false;
private:
	_bool				m_bSetActive = false;
	_float				m_fActiveTimer = 0;
	_bool				m_bCollisionCheck = false;

	_bool				m_bTest = false;
	_float				m_fAlphaTimer = 0.f;

public:
	static CTeleCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free()override;

};

