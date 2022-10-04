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
	//~

	//개별 함수
private:
	void		AttackJudge(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);
	//~

	//생성 소멸
public:
	static CSpider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);
	//~
private:
	int m_iPreIndex = 0;
	float m_fAttackDelay = 0;
	float m_fAttackDelayTime = 0;
	bool m_bAttack = true;
	bool m_bAttacking = false;

	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CAnimation*		m_pAttackAnimationCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
};

