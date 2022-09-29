#pragma once
#include "GameObject.h"


namespace Engine
{
	class CTransform;
	class CDynamic_Transform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CAnimation;

	class CCharacterInfo;
	class CCollider;

}

class CTestPlayer :public CGameObject
{
private:
	explicit CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);
	void				Set_OnTerrain(void);
	_vec3				PickUp_OnTerrain(void);


	// Bullet
	HRESULT				Create_Bullet(_vec3 vPos);
	// ~Bullet

public:
	_uint				Get_Magazine(void) { return m_iMagazine; }
	_bool				Get_Shoot(void) { return m_bOneShot; }
	// Test
	_uint				Get_ChangeImage(void) { return m_iChangeImage; }
	_uint				Get_HpChange(void) { return m_iHpBarChange; }
	// ~Test

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CCharacterInfo*		m_pInfoCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	// _vec3
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;
	// ~_vec3

	// Alpha Blending 용도 (리소스 얻은 뒤 쓰일 예정)
	_float				m_fFrame = 0.f;
	// ~Alpha Blending 용도
	
	_uint			m_iCoolTime = 0;
	_bool			m_bOneShot = FALSE;
	_uint			m_iMagazine = 8;

	// ~Bullet

	// Jump
	_float				m_fNowPosHeight = 0.f; //현재 포지션 지형의 Y값
	_float				m_fJumpPower;			//점프력
	_bool				m_bJump = FALSE;		//지금 점프상태인가?
	// ~Jump

	// Dash
	_bool			m_bDash = FALSE;
	_uint			m_iCountDash = 15;
	TYPING_TYPE		m_tpType;
	// ~Dash

	_float				m_fDashPower = 0.f;		//대쉬 시 가속력
	_float				m_fBuffDashPower = 0.f; //가중된 가속력

	// Test
	_uint				m_iChangeImage = 1;
	_uint				m_iHpBarChange = 4;
	// ~Test

public:
	static CTestPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

