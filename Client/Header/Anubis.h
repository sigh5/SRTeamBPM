#pragma once
#include "MonsterBase.h"

BEGIN(Engine)
class CCollider;
END


class CAnubis :public CMonsterBase
{
private:
	explicit CAnubis(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubis();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;
	void				Clear_Blood(const _float& fTimeDelta);


public:
	// 처형 판독
	virtual void		Excution_Event();

	void AttackJudge(const _float& fTimeDelta);
	// 공격
	void Attack(const _float& fTimeDelta);

public:
	CCalculator*		Get_Calculator(void) { return m_pCalculatorCom; }
	CRcTex*				Get_Buffer(void) { return m_pBufferCom; }

public:
	static CAnubis*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void		Free(void);

private:

	int m_iPreIndex = 0;
	float m_fAttackDelay = 0;
	float m_fAttackDelayTime = 0;
	bool m_bAttack = true;
	bool m_bAttacking = false;


	
	_vec3 m_bOldPlayerPos;

	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CTexture*		m_pDeadTextureCom = nullptr;

	CAnimation*		m_pAttackAnimationCom = nullptr;


	CRcTex*			m_pBufferCom = nullptr;

	//list<CAnubisThunder*> m_AnubisThunderlist;

};

