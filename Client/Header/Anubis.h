#pragma once
#include "MonsterBase.h"

BEGIN(Engine)
class CCollider;

END
class CAnubisThunder;
class CAnubisStormBall;

class CAnubis :public CMonsterBase
{
private:
	explicit CAnubis(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubis();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

	virtual bool		Dead_Judge(const _float& fTimeDelta);

	virtual void NoHit_Loop(const _float& fTimeDelta);
	virtual void Hit_Loop(const _float& fTimeDelta);

	void				Attack_Thunder(const _float& fTimeDelta);
	void				Attack_Stormball(const _float& fTimeDelta);

	void				Clear_Blood(const _float& fTimeDelta);
	void				Drop_Item(int ItemType);
public:
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();

	void AttackJudge(const _float& fTimeDelta);
	// АјАн
	void Attack(const _float& fTimeDelta);

public:
	CCalculator*		Get_Calculator(void) { return m_pCalculatorCom; }
	CRcTex*				Get_Buffer(void) { return m_pBufferCom; }

public:
	static CAnubis*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void		Free(void);

private:

	_vec3 m_bOldPlayerPos;

	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CTexture*		m_pDeadTextureCom = nullptr;

	CAnimation*		m_pAttackAnimationCom = nullptr;


	CRcTex*			m_pBufferCom = nullptr;

	list<CAnubisThunder*> m_AnubisThunderlist;
	list<CAnubisStormBall*> m_AnubisStormballList;
	//Thunder
	bool			m_bCreateOneThunder = false;
	bool			m_bCreateTwoThunder = false;

	int				m_iAttackPattern = 0;
	//~

	//Stormball
	int				m_iRepeatCharge = 0;
	int				m_iRepeatChargeNum = 0;
	int				m_iRepeatShoot = 0;
	int				m_iRepeatShootNum = 0;
	bool			m_bStormballLeftRight = false;
	bool			m_bCreateChargThunder = false;
	bool			m_bCreateOneStormball = false;
	//~

};

