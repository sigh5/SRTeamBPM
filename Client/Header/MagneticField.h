#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CTexture;
class CSphereTex;
class CTransform;
class CCalculator;
class CCollider;
class CMonsterBase;
END


class CMagneticField : public CGameObject
{
private:
	explicit CMagneticField(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CMagneticField();

public:
	HRESULT				Ready_Object(const _vec3& vCenter);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object()override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event() override;


public:
	void				Init_Setting(_vec3 vPos);


public:
	void				Set_Active(_bool bActive) { m_bSetActive = bActive; }
	const _bool&		Get_Active() { return m_bSetActive; }

private:
	HRESULT				Add_Component(void);

public:
	CSphereTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	_bool				m_bSetActive = false;
	_bool				m_bPlayerInTerrain = false;
	_int				m_iRestMonsterNum = 0;
	_float				m_fCollisionTimer = 0.f;

	_float				m_fMonsterSpawnTimer = 0.f;

	_float				m_fFrame = 0.f;

	_int				m_iMonsterMaxNum = 0;

public:
	static CMagneticField* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vCenter);
	virtual void		 Free()override;

};

