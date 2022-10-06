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
	int&				 Get_MonsterType() { return m_iMonsterIndex; }
	CharacterInfo&	     Get_InfoRef();
	void			     Get_MonsterToPlayer_Distance(float* _Distance);		// 몬스터 길이 구하는것
	virtual void		 Excution_Event() {};


public:
	const _bool&		 Get_Hit()const { return m_bHit; }


protected:
	CCharacterInfo*		m_pInfoCom = nullptr;    // 남길것 
	CAnimation*			m_pAnimationCom = nullptr;	 // 남길것 
	CDynamic_Transform* m_pDynamicTransCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
	CAnimation*		m_pDeadAnimationCom = nullptr;



protected:
	_int	m_iMonsterIndex;
	_bool	m_bHit = false;
	_float	m_fHitDelay;
	_int	m_iPreHp;
	_float			m_fTimeDelta = 0.f;
	vector<CGameObject*> m_vecBlood;
	_vec3		m_vPlayerPos, m_vMonsterPos;
	bool m_bDead = false;


protected:
	virtual HRESULT				Add_Component(void);
	virtual bool				Set_TransformPositon(HWND g_hWnd, CCalculator* _pCalcul);
	
protected:
	_float fMtoPDistance =20.f; // 몬스터와 플레이어 간의 거리

public:
	virtual void		Free(void);
};

END