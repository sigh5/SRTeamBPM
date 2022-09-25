#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
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
	float				Get_TerrainY(void);		//지형의 Y값으로 설정하는 대신 리턴

	// Bullet
	HRESULT				Create_Bullet(_vec3 vPos);
	// ~Bullet

	// Dash
	void				Dash(const _float& fTimeDelta);
	// ~Dash

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	CScene*				m_pSceneCom = nullptr;
	CManagement*		m_pManageCom = nullptr;

private:
	// _vec3
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;
	// ~_vec3

	// Alpha Blending 용도 (리소스 얻은 뒤 쓰일 예정)
	_float				m_fFrame = 0.f;
	// ~Alpha Blending 용도

	// Bullet 	
	list<_tchar*>	m_szBulletName;

	_uint			m_iCount = 0;
	_bool			m_bOneShot = false;
	_uint			m_iCoolTime = 0;

	_uint			m_iMagazine = 8;
	// ~Bullet



	_float				m_fNowPosHeight = 0.f; //현재 포지션 지형의 Y값

	_bool				m_bJump = FALSE;		//지금 점프상태인가?

	_float				m_fJumpPower;			//점프력

	// Dash
	_bool			m_bDash = false;
	_uint			m_iCountDash = 0;
	// ~Dash

	float				m_fDashPower = 0.f;		//대쉬 시 가속력
	float				m_fBuffDashPower = 0.f; //가중된 가속력

public:
	static CTestPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

