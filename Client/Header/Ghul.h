#pragma once
#include "MonsterBase.h"
class CGhul :
	public CMonsterBase
{
private:
	explicit CGhul(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGhul();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

	virtual bool		Dead_Judge(const _float& fTimeDelta);

	virtual void NoHit_Loop(const _float& fTimeDelta);
	virtual void Hit_Loop(const _float& fTimeDelta);

	void AttackJudge(const _float& fTimeDelta);
	// АјАн
	void Attack(const _float& fTimeDelta);
	
	void DigOut(const _float& fTimeDelta);

public:
	static CGhul*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void		Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CTexture*		m_pDeadTextureCom = nullptr;
	CTexture*		m_pGhulDigoutTextureCom = nullptr;

	CAnimation*		m_pAttackAnimationCom = nullptr;
	CAnimation*		m_pDigoutAnimationCom = nullptr;

	CRcTex*			m_pBufferCom = nullptr;

	bool m_bCanDie = false;
	bool m_bCanWalk = false;
};

