#pragma once
#include "MonsterBase.h"
class CEarthSpike;
class CEarthShaker :
	public CMonsterBase
{
private:
	explicit CEarthShaker(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEarthShaker();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();
	virtual bool		Dead_Judge(const _float& fTimeDelta);
	void				Ready_Attack(const _float& fTimeDelta);
	virtual void		NoHit_Loop(const _float& fTimeDelta);
	virtual void		Hit_Loop(const _float& fTimeDelta);
	virtual void		Excution_Event();
private:
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);
	void		SpikeUpdateLoop(const _float& fTimeDelta);
	void	Drop_Item(int ItemType);
public:
	static CEarthShaker*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0);
	virtual void	Free(void);


	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CTexture*		m_pDeadTextureCom = nullptr;

	CAnimation*		m_pAttackAnimationCom = nullptr;

	int			m_iAttacknumber = 0;
	int			m_iReadyAttackNumber = 0;
	int			m_iDefenseless = 0;
	bool		m_bReady_Attack = false;
	bool		m_bAttackWaiting = false;
	bool		m_bReadyAttackNumber = false;
	bool		m_bQoongSound = false;
	bool		m_bAttackSound = false;

	bool		m_bSpikeCreate = false;


	list<CEarthSpike*>	m_Spikelist;
	float		m_fInterval = 0.f;
	//float		m_fOriginInterval = 0.f;
	float		m_fWaitingTime = 0.f;
	//float		m_fOriginwaiting = 0.f;
	bool		m_bSpikeType = false;
	float		m_fSoundInterval = 0.f;
	float		m_fSoundCount = 0.f;
	int			m_iSoundNumber = 0;
};

