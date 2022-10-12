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
	virtual HRESULT		Ready_Object(float Posx, float Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

	virtual bool		Dead_Judge(const _float& fTimeDelta);

	virtual void NoHit_Loop(const _float& fTimeDelta);
	virtual void Hit_Loop(const _float& fTimeDelta);



	void				Clear_Blood(const _float& fTimeDelta);
public:
	// ó�� �ǵ�
	virtual void		Excution_Event();

	void AttackJudge(const _float& fTimeDelta);
	// ����
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

};

