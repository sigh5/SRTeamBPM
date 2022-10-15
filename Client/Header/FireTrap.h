#pragma once
#include "MonsterBase.h"
class CFirePillar;
class CFireTrap :
	public CMonsterBase
{
private:
	explicit CFireTrap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireTrap();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy);
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

	list<CFirePillar*> m_FirePillarlist;

public:
	static CFireTrap*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0);
	virtual void		Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;

	CRcTex*			m_pBufferCom = nullptr;
	bool			m_bCreateOneFire = false;
};

