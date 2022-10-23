#pragma once

#include "Engine_Include.h"
#include "GameObject.h"

namespace Engine
{
	class CDynamic_Transform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CAnimation;
	class CCharacterInfo;
	class CTransform;
	class CCollider;
}


class CMiniMonster2 :public CGameObject
{
private:
	explicit CMiniMonster2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniMonster2();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Collision_Event()override;

	void	Set_Dead(_bool bEvent) { m_bDead = bEvent; }
	virtual		_vec3		Get_Pos();
	virtual		void		Move_Pos(_vec3 _move);
private:
	void			Judge_Active(const _float& fTimeDelta);
	
	_int			CalCulate_Distacne(CTransform* pPlayerTransCom, const _float& fTimeDelta);	// 0 chase 1 attack 2 stop

public:
	void	Set_Pos(const _vec3& vPos);
private:
	HRESULT			Add_Component();
public:
	static CMiniMonster2*	Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posy = 0);
	virtual void			Free(void);

private:
	CTexture*		m_pTextureCom = nullptr;
	CTexture*		m_pAttackTextureCom = nullptr;
	CTexture*		m_pGhulDigoutTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;	 // ³²±æ°Í 
	CAnimation*		m_pAttackAnimationCom = nullptr;
	CAnimation*		m_pDigoutAnimationCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	bool m_bCanDie = false;
	bool m_bCanWalk = false;

	bool	m_bDigOutSound = false;
	bool	m_bAttackSound = false;

	_bool		m_bAttack = false;
	_bool		m_bChase = true;

	_float	m_fFrame = 0.f;
};

