#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCharacterInfo;
class CTransform;
class CRcTex;
class CTexture;
class CAnimation;
class CCalculator;
class CDynamic_Transform;
class CCollider;

class ENGINE_DLL CMonsterBase :public CGameObject
{
protected:
	explicit CMonsterBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterBase(const CMonsterBase& rhs);
	virtual ~CMonsterBase();

public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;
	virtual void		 Collision_Event()override {}

public:
	bool				 Set_SelectGizmo(HWND g_hWn, CCalculator* _pCalcul, CRcTex* _pBuffer);
	MONSTER_INDEX&				 Get_MonsterType() { return m_iMonsterIndex; }
	CharacterInfo&	     Get_InfoRef();
	void			     Get_MonsterToPlayer_Distance(float* _Distance);		// 몬스터 길이 구하는것
	virtual void		 Excution_Event() {};
	virtual bool		 Dead_Judge(const _float& fTimeDelta) { return 0; }
	virtual void		Hit_Delay_toZero(void);

	virtual void NoHit_Loop(const _float& fTimeDelta) {};
	virtual void Hit_Loop(const _float& fTimeDelta) {};

	virtual bool		Distance_Over(void);
	virtual void		Save_OriginPos(void);
	virtual void		Get_BackOriginPos(void);
	virtual float		Get_Radius(void);
	virtual _vec3		Get_Pos(void);
	virtual void		Move_Pos(_vec3 _move);
	virtual void		Add_ColliderMonsterlist(void);

public:
	const _bool&		 Get_Hit()const { return m_bHit; }
	const _bool&		Get_Dead()const { return m_bDead; }


protected:
	CCharacterInfo*		m_pInfoCom = nullptr;    // 남길것 
	CAnimation*			m_pAnimationCom = nullptr;	 // 남길것 
	CDynamic_Transform* m_pDynamicTransCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
	CAnimation*		m_pDeadAnimationCom = nullptr;



protected:
	MONSTER_INDEX	m_iMonsterIndex;
	_bool	m_bHit;
	_float	m_fHitDelay;
	_int	m_iPreHp;
	_float			m_fTimeDelta;
	vector<CGameObject*> m_vecBlood;
	_vec3		m_vPlayerPos, m_vMonsterPos;

	_bool	m_bResetCheck;

	_bool	m_bExcutionCheck;



	bool m_bDead = false;
	_vec3	m_vOriginPos;
	_int	m_iOriginHp;

	int m_iPreIndex;
	float m_fAttackDelay;
	float m_fAttackDelayTime;
	bool m_bAttack;
	bool m_bAttacking;


protected:
	virtual HRESULT				Add_Component(void);
	virtual bool				Set_TransformPositon(HWND g_hWnd, CCalculator* _pCalcul);

protected:
	_float fMtoPDistance; // 몬스터와 플레이어 간의 거리

public:
	virtual void		Free(void);
};

END