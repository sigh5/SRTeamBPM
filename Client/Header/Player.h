#pragma once
#include "GameObject.h"
#include "Engine_Include.h"


namespace Engine
{
	class CDynamic_Transform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CAnimation;
	class CCharacterInfo;
	class CCollider;
}


class CPlayer :public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();


public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Collision_Event();

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);

public:
	void				Set_OnTerrain(void);
	_vec3				PickUp_OnTerrain(void);


public:

	_bool				Create_RayCheck(const _float & fTimeDelta); //레이피킹
	// SoundCheck
	_bool				Get_SoundCheck() { return m_bSoundCheck; }
	void				Set_SoundCheck(_bool bSoundCheck) { m_bSoundCheck = bSoundCheck; }
	_int				Get_ComboCount() { return m_iComboCount; }
	void				Set_ComboCount(_int iCount) { m_iComboCount += iCount; }
	_bool				Get_Shoot(void) { return m_bOneShot; }
	_uint				Get_Magazine(void) { return m_iMagazine; }
	void				Set_OneShot(_bool bMetro) { m_bOneShot = bMetro; }
	_bool				Get_CheckShot() { return m_bCheckShot; }
	void				Set_CheckShot(_bool bCheckShot) { m_bCheckShot = bCheckShot; }

public:
	_uint				Get_HpChange(void) { return m_iHpBarChange; }
	_uint				Get_Skill(void) { return m_iSkillPower; }
	

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCharacterInfo*		m_pInfoCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:		// Jw
	// _vec3
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;
	_float				m_fFrame = 0.f;
	// ~_vec3
	// Alpha Blending 용도 (리소스 얻은 뒤 쓰일 예정)

	_bool				m_bOneShot = false;		// 1 클릭 사격 횟수 제한
	_uint				m_iMagazine = 8;		// 탄창
											// ~Bullet
	// Jump
	_float				m_fNowPosHeight = 0.f; //현재 포지션 지형의 Y값
	_float				m_fJumpPower;			//점프력
	_bool				m_bJump = false;		//지금 점프상태인가?
	// ~Jump

	// Dash
	_bool				m_bDash = false;
	_uint				m_iCountDash = 15;
	TYPING_TYPE			m_tpType;					// 어느 방향으로 가는 중인가에 따라 대쉬 방향을 정하기 위한 구조체 변수
	// ~Dash

	_float				m_fDashPower = 0.f;		//대쉬 시 가속력
	_float				m_fBuffDashPower = 0.f; //가중된 가속력
	




	_uint				m_iSkillPower = 1;
	_uint				m_iHpBarChange = 4;



private: //hw
	_bool			m_bCheckShot = false;
	_bool			m_bMissCheck = false;
	_bool			m_bSoundCheck = false;
	_int			m_iComboCount = 0;

	_float			m_fTimeDelta = 0.f;
	DIR				m_eDirType = DIR_END;


public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

