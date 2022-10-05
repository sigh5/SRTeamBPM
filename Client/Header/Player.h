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

	_bool				Create_RayCheck(const _float & fTimeDelta); //������ŷ
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
	// Alpha Blending �뵵 (���ҽ� ���� �� ���� ����)

	_bool				m_bOneShot = false;		// 1 Ŭ�� ��� Ƚ�� ����
	_uint				m_iMagazine = 8;		// źâ
											// ~Bullet
	// Jump
	_float				m_fNowPosHeight = 0.f; //���� ������ ������ Y��
	_float				m_fJumpPower;			//������
	_bool				m_bJump = false;		//���� ���������ΰ�?
	// ~Jump

	// Dash
	_bool				m_bDash = false;
	_uint				m_iCountDash = 15;
	TYPING_TYPE			m_tpType;					// ��� �������� ���� ���ΰ��� ���� �뽬 ������ ���ϱ� ���� ����ü ����
	// ~Dash

	_float				m_fDashPower = 0.f;		//�뽬 �� ���ӷ�
	_float				m_fBuffDashPower = 0.f; //���ߵ� ���ӷ�
	




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

