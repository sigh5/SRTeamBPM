#pragma once
#include "MonsterBase.h"
class CFinalBoss :public CMonsterBase
{
private:
	explicit CFinalBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalBoss();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, float Size);
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

public:
	static CFinalBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0, float Size = 0);
	virtual void	Free(void);

private:
	CTexture*		m_pCamouTextureCom = nullptr;
	CTexture*		m_pCamouAttackTextureCom = nullptr;

	CRcTex*			m_pBufferCom = nullptr;
	bool			m_bCamouflage = true;

	int				m_iAttackPattern = 0;

};

