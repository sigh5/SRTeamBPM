#pragma once
#include "MonsterBase.h"


class CSphinx :public CMonsterBase
{
private:
	explicit CSphinx(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphinx();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();

	void			BattleLoop(const _float& fTimeDelta);
	void			IdleLoop(const _float& fTimeDelta);

private:
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);

public:
	static CSphinx*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);

private:

	_vec3 m_vOldPlayerPos;
	bool m_bBattle = false;
	bool m_bHeadOff = false;
	int		m_iShootLeftRight;
	int		m_iShootCycle;



	int		m_iAliveStatue;    //남아있는 비석의 갯수

	_vec3 m_vScale;

private:


	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CAnimation*		m_pAttackAnimationCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
};

