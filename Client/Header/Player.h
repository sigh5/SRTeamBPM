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

class CGun_Screen;
//주석지우셈
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
	
	_vec3				PickUp_OnTerrain(void);


public:
	void				Ready_MonsterShotPicking(); //레이피킹

	_int				Get_ComboCount() { return m_iComboCount; }
	void				Set_ComboCount(_int iCount) { m_iComboCount += iCount; }
	void				Reset_ComboCount() { m_iComboCount = 0; }

	inline void			Push_Hitcheck(_bool _bhitCheck) { if (m_bMissCheck){return;}m_bMissCheck = _bhitCheck;}
	void				ComboCheck();


	void				EquipItem_Add_Stat(void);

public:					// ★ m_pInfoCom->Get_InfoRef()._iHp를 return 해주면 실전 OK
	_uint				Get_HpChange(void) { return m_iOriginHP; }
	_uint				Get_Skill(void) { return m_iSkillPower; }
	

	void				Set_bPreStat(_bool _AddStat) { m_bPreStat = _AddStat; }
	void				Set_bCurStat(_bool _bStat) { m_bCurStat = _bStat; }

	void				Loss_Damage();

	void				Excution_Motion();

	void				Set_inventroyActive(_bool bInventoryActive) { m_bInventroyActive = bInventoryActive; }
	const _bool&		Get_inventroyActive() { return m_bInventroyActive; }

private:
	void				Random_ResurrectionRoom();
	void				Player_Dead_CaemraAction();
	void				Player_Dead(const _float& fTimeDelta);

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

	// Jump
	_float				m_fNowPosHeight = 0.f; //현재 포지션 지형의 Y값
	_float				m_fJumpPower;			//점프력
	_bool				m_bJump = false;		//지금 점프상태인가?
	// ~Jump

	// Dash
	_bool				m_bDash = false;
	_uint				m_iCountDash = 15;
	TYPING_TYPE			m_tpType;					// 어느 방향으로 가는 중인가에 따라 대쉬 방향을 정하기 위한 구조체 변수
	_float				m_fDashPower = 0.f;		//대쉬 시 가속력
	_float				m_fBuffDashPower = 0.f; //가중된 가속력
	// ~Dash

	// Player_Combo
	_int				m_iComboCount = 0;
	_bool				m_bMissCheck = false;
	// Player_Combo											
	

	_uint				m_iSkillPower = 1;
	_uint				m_iHpBarChange = 100;
	
	
	
	_float				m_fTimeDelta = 0.f;
	_int				m_iOriginHP = 0;


	// Gun
	CGun_Screen*		pEquipItem;

	// Player's Status(Private)	
	_bool				m_bPreStat = false;
	_bool				m_bCurStat = false;
	_bool				m_bGainItem[3] = { false, }; //나중에 숫자만큼 넣으세요

	// ~Player's Status(Private)

	// InventroyActive 
	_bool				m_bInventroyActive = false;
	_float				m_bDeadTimer = 0.f;
	// ~InventroyActive 


public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

