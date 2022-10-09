#pragma once
#include "MonsterBase.h"
class CObelisk : public CMonsterBase
{
private:
	explicit CObelisk(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObelisk();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

	bool			Get_Dead() { return m_bDead; }
private:
	virtual bool		Dead_Judge(const _float& fTimeDelta);

	void AttackJudge(const _float& fTimeDelta);

	void Attack(const _float& fTimeDelta);

	virtual void NoHit_Loop(const _float& fTimeDelta);
	virtual void Hit_Loop(const _float& fTimeDelta);
public:
	static CObelisk*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void		Free(void);

private:
	CTexture* m_pTextureCom = nullptr;
	CRcTex* m_pBufferCom = nullptr;

};

