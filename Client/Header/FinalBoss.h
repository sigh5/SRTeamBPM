#pragma once
#include "MonsterBase.h"
enum FinalBossState
{
	Camouflage_Walk, Camouflage_Shoot, Morph, Thingy_Walk, Attack_A, Attack_B, Attack_C ,Attack_D , Thingy_Death, State_END
};
class CFinalBoss :public CMonsterBase
{
private:
	explicit CFinalBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalBoss();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();
	virtual bool		 Dead_Judge(const _float& fTimeDelta);

	void		Set_Pettern3Finish() { m_bPettern3Finish = true; }
	bool		Get_Pettern3Finish() { return m_bPettern3Finish; }
	void		Set_OnekiokGo() { m_bOnekiokGo = true; }

	void		Camouflage_Attack(const _float& fTimeDelta);
	void		Camouflage_Cancle(const _float& fTimeDelta);
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);
	void		BattleLoop(const _float& fTimeDelta);
	void		AttackPettern1(const _float& fTimeDelta);
	void		AttackPettern2(const _float& fTimeDelta);
	void		AttackPettern3(const _float& fTimeDelta);
	void		AttackPettern4(const _float& fTimeDelta);

public:
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();
public:
	static CFinalBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0);
	virtual void	Free(void);

private:
	CTexture*		m_pCamouTextureCom = nullptr;
	CTexture*		m_pCamouAttackTextureCom = nullptr;
	CTexture*		m_pMorphTextureCom = nullptr;
	CTexture*		m_pThingy_AttackATextureCom = nullptr;
	CTexture*		m_pThingy_AttackBTextureCom = nullptr;
	CTexture*		m_pThingy_AttackCTextureCom = nullptr;
	CTexture*		m_pThingyTextureCom = nullptr;
	CTexture*		m_pThingy_OnekiokTextureCom = nullptr;
	CTexture*		m_pDeadTextureCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;

	CAnimation*		m_pCamouAttackAnimationCom = nullptr;
	CAnimation*		m_pMorphAnimationCom = nullptr;
	CAnimation*		m_pThingy_AttackA_AnimationCom = nullptr;
	CAnimation*		m_pThingy_AttackB_AnimationCom = nullptr;
	CAnimation*		m_pThingy_AttackC_AnimationCom = nullptr;
	CAnimation*		m_pThingy_Onekiok_AnimationCom = nullptr;
	CAnimation*		m_pThingy_AnimatiionCom = nullptr;

	bool			m_bCamouflage = true;
	bool			m_bMorphFinish = false;
	bool			m_bMorphSound = false;

	int				m_iAttackPattern = 0;
	FinalBossState  m_bState = Camouflage_Walk;
	bool			m_bShotBullet = false;
	bool			m_bAttackSound = false;

	//AttackPettern1
	bool			m_bOnekiokGo = false;
	bool			m_bCreatedOnekiok = false;

	//AttackPettern2
	bool			m_bCreatedTapeWorm = false;
	//~

	//AttackPettern3
	int				m_iHitNum = 0;
	bool			m_bTentacleAnimation = false;
	bool			m_bCreateTentacle = false;
	bool			m_bPettern3Finish = false;
	float			m_fWaitingTime = 0;
	//~

	//AttackPattern4
	int				m_iPettern4RepeatNum = 0;
	float			m_fPlusRight = 0.f;
	float			m_fMinuRight = 0.f;
	bool			m_bCreateSpike = false;
	_vec3			m_vPlayerOriginPos;
	_vec3			m_vMonsterRight, m_vMonsteFront, m_vMonsterLeft, m_vMonsterback;
	float			m_fFront, m_fRight, m_fBack, m_fLeft;
	bool			m_bSavePosition = false;
	int				m_iOneDirrection = 0;
	bool			m_bPettern4LR = false;

	bool			m_bShootFlare = false;
	int				m_iRepeatShot = 0;
};

