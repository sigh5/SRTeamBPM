#pragma once
#include "MonsterBase.h"
class CObelisk : public CMonsterBase
{
private:
	explicit CObelisk(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObelisk();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

	virtual void		Get_BackOriginPos(void);

	bool			Get_Dead() { return m_bDead; }
	bool			Get_Hit() { return m_bHit; }

public:
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();
private:
	virtual bool		Dead_Judge(const _float& fTimeDelta);

	void AttackJudge(const _float& fTimeDelta);

	void Attack(const _float& fTimeDelta);
	void	Drop_Item(int ItemType);

	virtual void NoHit_Loop(const _float& fTimeDelta);
	virtual void Hit_Loop(const _float& fTimeDelta);
public:
	static CObelisk*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0);
	virtual void		Free(void);

private:
	CTexture* m_pTextureCom = nullptr;
	CRcTex* m_pBufferCom = nullptr;
	bool	m_bSpawnSound = nullptr;
	_vec3	m_vOriginScale;

};

