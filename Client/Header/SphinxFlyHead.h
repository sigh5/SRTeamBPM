#pragma once
#include "MonsterBase.h"
class CSphinxFlyHead :
	public CMonsterBase
{
private:
	explicit CSphinxFlyHead(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphinxFlyHead();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, float Size);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();
	virtual bool		 Dead_Judge(const _float& fTimeDelta);

	void				Dead_Action(const _float& fTimeDelta);

	void		HeadActive(const _float& fTimeDelta);
	void		 AttackJudge(const _float& fTimeDelta);
	void		BattleLoop(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);
	//LeftRight
	void		AttackLeftRight(const _float& fTimeDelta);
	void		LeftRightJudge(const _float& fTimeDelta);
	void		LeftAttack(const _float& fTimeDelta);
	void		RightAttack(const _float& fTimeDelta);
	void		Save_PlayerPos_forLR(const _float& fTimeDelta);
	void		Save_PlayerPos_forBody(const _float& fTimeDelta);
	//~

	//bodyattack
	void		BodyAttack(const _float& fTimeDelta);
	void		Tackle(const _float& fTimeDelta);
	void		Tackle_HeadSpin(const _float& fTimeDelta);
	void		Rearrangement(const _float& fTimeDelta);
	//~
public:
	static CSphinxFlyHead*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0, float Size = 0);
	virtual void	Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;

	CRcTex*			m_pBufferCom = nullptr;
	bool	m_bBattle = false;
	float m_fDeadY = 0.f;
	bool  m_bArrFalldown[4];
	float m_fLimitY = 0.f;
	float	m_fVolume = 0.f;
	int		m_iAttackPattern = 0;

	//dead
	CTexture*		m_pDeadTexture = nullptr;
	//~

	//HeadActivated
	CTexture*		m_pActivatedTextureCom = nullptr;
	CAnimation*		m_pHeadActivatedAnimationCom = nullptr;
	//~

	//LRAttack
	CTexture*		m_pLRAttackTextureCom = nullptr;
	CAnimation*		m_pLRAttackAnimationCom = nullptr;
	int		m_iLRJudge = 0;
	bool	m_bGet_PlayerPos_LR = false;
	bool	m_bSelectedLeftRight = false;
	_vec3  m_vPrePlayerPosdir;
	bool	m_bLRChargeSound = false;
	bool	m_bLRAttackSound = false;
	//~

	//BodyAttack
	CTexture*		m_pBodyAttackTextureCom = nullptr;
	CAnimation*		m_pBodyAttackAnimation = nullptr;
	bool		m_bBodyAttackChargeFinish = false;
	bool		m_bGet_PlayerPos_Body = false;
	bool		m_bTackleStart = false;
	float		m_fTackleSpeed = 0;

	float		m_fTackleAttenuationTime = 0;
	float		m_fTackleAttenuationTimeCount = 0;
	float		m_fTackleStopper = 0;
	float		m_fOriginTackleStopper = 0;
	bool		m_bRenderBodyAttack = false;
	int			m_iBodyAttackLevel = 0;
	bool		m_bAttenuationStart = false;
	bool		m_bFrontback = false;
	bool		m_bLeftRight = false;
	_vec3  m_vTargetPos;
	_vec3  m_vTackleDir;
	bool	m_bHitPlayer = false;
	bool	m_bBodyAttackSound = false;
	bool	m_bChargeSound = false;
	//~
	float m_fRearrangementDelay = 0.f;
	float m_fRearrangementDealyCount = 0.f;
	

};

