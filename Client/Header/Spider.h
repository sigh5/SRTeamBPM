#pragma once
#include "MonsterBase.h"




class CSpider :public CMonsterBase
{
private:
	explicit CSpider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpider();

	//상속
public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();
	virtual void		Excution_Event(_bool bAOE = false)override;
	virtual bool		Dead_Judge(const _float& fTimeDelta);

	virtual void NoHit_Loop(const _float& fTimeDelta);
	virtual void Hit_Loop(const _float& fTimeDelta);
	//~

	//개별 함수
private:
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);
	void			Drop_Item(int ItemType);
	//~

public:
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();

	//생성 소멸
public:
	static CSpider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);
	//~
private:
	// 데미지 HW
	_bool			m_bHitDamage = true;
	bool	m_bReadyAttackSound = false;
	bool	m_bAttackSound = false;
	bool	m_bAttackSound2 = false;

private:
	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CTexture*		m_pDeadTextureCom = nullptr;
	CAnimation*		m_pAttackAnimationCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	

};

