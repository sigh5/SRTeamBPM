#pragma once
#include "MonsterBase.h"
class CEarthShaker :
	public CMonsterBase
{
private:
	explicit CEarthShaker(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEarthShaker();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, float Size);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();
	virtual bool		 Dead_Judge(const _float& fTimeDelta);
	void			Ready_Attack(const _float& fTimeDelta);
	virtual void NoHit_Loop(const _float& fTimeDelta);
private:
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);

public:
	static CEarthShaker*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0, float Size = 0);
	virtual void	Free(void);

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
};
