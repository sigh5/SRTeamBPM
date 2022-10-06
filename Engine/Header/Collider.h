#pragma once
#include "Component.h"


BEGIN(Engine)

class CGameObject;
class CRcTex;
class CTransform;


class ENGINE_DLL CCollider :public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	HRESULT		Ready_Collider(void);
	virtual _int			Update_Component(const _float& fTimeDelta);
	virtual void			LateUpdate_Component(void);
	
	void			Render();
	

public:
	
	_bool		Check_Sphere_InterSect(_vec3 vCenter1, _vec3 vCenter2, _float fRadius1, _float fRadius2);

	_bool		Check_Lay_InterSect(CRcTex * rcTex, CTransform* pMonsterCom, HWND hWnd);
	_bool		Check_Lay_InterSect(CRcTex * rcTex, CTransform* pMonsterCom, HWND hWnd, _vec3& PickPos);
	
	// 이동관련
	_bool		Check_Collision_Wall(CGameObject* pCubeObject, CGameObject* pDest );
	
	_bool		Check_collision_Wall_AABB(CCollider* pCube, CCollider* pObject);


	_bool		Check_Collision(CGameObject* pItemObject, CGameObject* pPlayer,const _float& fItemRadius,const _float& fPlayerRadius);


public:
	void		Set_vMax_vMin(_vec3 *vMin, _vec3*vMax) {m_vMin = *vMin;	m_vMax = *vMax;}
	
	const _vec3&	Get_MinVec()const { return m_vMin; }
	const _vec3&	Get_MaxVec()const { return m_vMax; }

public:
	virtual CComponent* Clone(void) override;
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	_bool	   Check_Collision_AABB(_vec3 *vMin1, _vec3 *vMax1, _vec3 *vMin2, _vec3 *vMax2);



private:
	bool		m_bClone;

	_vec3		m_vMin;
	_vec3		m_vMax;



private:
	virtual void Free(void) override;
};

END