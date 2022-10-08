#pragma once
#include "MonsterBase.h"
class CSphinxFlyHead :
	public CMonsterBase
{
private:
	explicit CSphinxFlyHead(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphinxFlyHead();

public:
	virtual HRESULT		Ready_Object(float Posx, float Posy, float Size);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event();


	void		HeadActive(const _float& fTimeDelta);
	void		 AttackJudge(const _float& fTimeDelta);
	void		BattleLoop(const _float& fTimeDelta);
	void		Attack(const _float& fTimeDelta);
	//LeftRight
	void		AttackLeftRight(const _float& fTimeDelta);
	void		LeftRightJudge(const _float& fTimeDelta);
	void		LeftAttack(const _float& fTimeDelta);
	void		RightAttack(const _float& fTimeDelta);
	//~

public:
	static CSphinxFlyHead*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0, float Size = 0);
	virtual void	Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;
	//HeadActivated
	CTexture*		m_pActivatedTextureCom = nullptr;
	CAnimation*		m_pHeadActivatedAnimationCom = nullptr;

	//~

	//LRAttack
	CTexture*		m_pLRAttackTextureCom = nullptr;
	CAnimation*		m_pLRAttackAnimationCom = nullptr;
	bool	m_bLRJudge = false;
	bool	m_bGet_PlayerPos = false;
	bool	m_bSelectedLeftRight = true;
	_vec3  m_vPrePlayerPosdir;
	//~

	int		m_iAttackPattern;
	
	CRcTex*			m_pBufferCom = nullptr;
	
	bool	m_bBattle = false;



	
};

