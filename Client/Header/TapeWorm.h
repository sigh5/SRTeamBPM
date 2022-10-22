#pragma once
#include "MonsterBase.h"
class CTapeWorm : public CMonsterBase
{
private:
	explicit CTapeWorm(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTapeWorm();

public:
	virtual HRESULT		Ready_Object(float	Posx, float Posz);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();
	
	virtual bool		Dead_Judge(const _float& fTimeDelta);

	virtual void NoHit_Loop(const _float& fTimeDelta);


private:
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);

public:
	static CTapeWorm*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posz = 0);
	virtual void	Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CTexture*		m_pDeadTextureCom = nullptr;
	CAnimation*		m_pAttackAnimationCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	
	bool			m_bHitPlayer = false;

};

