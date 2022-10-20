#pragma once
#include "MonsterBase.h"
enum FinalBossState
{
	Camouflage_Walk, Camouflage_Shoot, Morph, Thingy_Walk, Attack_A, Attack_B, Attack_C, Thingy_Death, State_END
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

	void		Camouflage_Attack(const _float& fTimeDelta);
	void		Camouflage_Cancle(const _float& fTimeDelta);
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);
	void		BattleLoop(const _float& fTimeDelta);
	void		AttackPettern2(const _float& fTimeDelta);

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
	CRcTex*			m_pBufferCom = nullptr;

	CAnimation*		m_pCamouAttackAnimationCom = nullptr;
	CAnimation*		m_pMorphAnimationCom = nullptr;
	CAnimation*		m_pThingy_AttackA_AnimationCom = nullptr;
	CAnimation*		m_pThingy_AttackB_AnimationCom = nullptr;
	CAnimation*		m_pThingy_AttackC_AnimationCom = nullptr;
	CAnimation*		m_pThingy_AnimatiionCom = nullptr;

	bool			m_bCamouflage = true;
	bool			m_bMorphFinish = false;

	int				m_iAttackPattern = 0;
	FinalBossState  m_bState = Camouflage_Walk;
	bool			m_bShotBullet = false;

	//AttackPettern2
	bool			m_bCreatedTapeWorm = false;
	//~

};

