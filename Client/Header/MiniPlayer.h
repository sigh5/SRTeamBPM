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

class CMiniPlayer :public CGameObject
{
private:
	explicit CMiniPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Collision_Event();

public:
	void			Set_MonsterKillCount() { m_iKillMonster += 1; }
	const _int&		Get_MonsterKillCount() { return m_iKillMonster; }


	_float			m_fSpeed = 2.f;

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);

	void				Set_OnTerrain();



private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CDynamic_Transform*	m_pDynamicTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;


	_bool				m_bDash = false;
	_uint				m_iCountDash = 15;
	TYPING_TYPE			m_tpType;					// 어느 방향으로 가는 중인가에 따라 대쉬 방향을 정하기 위한 구조체 변수
	_float				m_fDashPower = 0.f;		//대쉬 시 가속력
	_float				m_fBuffDashPower = 0.f; //가중된 가속력
	_float				m_fDashTimer = 0.f;
	_bool				m_bIsDash = false;

public:
	_int				m_iCreateBulletTime = 3;

private:		
	_vec3				m_vDirection;
	_vec3				m_vUp;
	_vec3				m_vPos;
	_float				m_fFrame = 0.f;


	_int				m_iKillMonster = 0;

	float				m_fPiercingBulletTime = 0.f;
	int					m_iPiercingBulletNum = 0;
	bool				m_bCanPiercing = false;
	bool				m_bCreatedNotice = false;
public:
	static CMiniPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);


};

