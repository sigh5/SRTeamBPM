#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CDynamic_Transform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CScene;
	class CManagement;
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


private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;


private:
	// _vec3
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;
	// ~_vec3

	// Alpha Blending �뵵 (���ҽ� ���� �� ���� ����)
	_float				m_fFrame = 0.f;
	// ~Alpha Blending �뵵

	// Bullet 	
	list<_tchar*>	m_szBulletName;

	_uint			m_iCount = 0;
	_bool			m_bOneShot = FALSE;
	_uint			m_iCoolTime = 0;

	_uint			m_iMagazine = 8;
	// ~Bullet



	_float				m_fNowPosHeight = 0.f; //���� ������ ������ Y��

	_bool				m_bJump = FALSE;		//���� ���������ΰ�?

	_float				m_fJumpPower;			//������

	// Dash
	_bool			m_bDash = FALSE;
	_uint			m_iCountDash = 15;

	TYPING_TYPE		m_tpType;
	// ~Dash

	float				m_fDashPower = 0.f;		//�뽬 �� ���ӷ�
	float				m_fBuffDashPower = 0.f; //���ߵ� ���ӷ�

public:
	static CTestPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

