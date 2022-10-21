#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CSphereTex;
class CTransform;
class CCalculator;
class CCollider;
class CMonsterBase;
END


class CControlRoom :public CGameObject
{
private:
	explicit CControlRoom(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CControlRoom();

public:
	HRESULT				Ready_Object(const _vec3& vCenter);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object()override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event() override;

public:
	void				Set_Active(_bool bActive) { m_bSetActive = bActive; }
	const _bool&		Get_Active() { return m_bSetActive; }


	void				Area_of_Effect();



private:
	HRESULT				Add_Component(void);

public:
	HRESULT				SetUp_Material(void);
	_bool				Set_Light_Obj();


public:
	CSphereTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	


private:
	vector<CGameObject*> m_pCurrentRoomMonster;



private:
	_bool				m_bSetActive = false;

	_bool				m_bPlayerInTerrain = false;
	_int				m_iRestMonsterNum = 0;


	_float				m_fCollisionTimer = 0.f;


public:
	static CControlRoom* Create(LPDIRECT3DDEVICE9 pGraphicDev,const _vec3& vCenter);
	virtual void		 Free()override;

};

