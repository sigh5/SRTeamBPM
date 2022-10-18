#pragma once
#include "Component.h"
#include "VIBuffer.h"

BEGIN(Engine)

class CGameObject;
class CRcTex;
class CTransform;
class CCubeTex;

class ENGINE_DLL CCollider :public CVIBuffer
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	
	HRESULT		Ready_Buffer(void);
	virtual void Render_Buffer(void) override;
	
	virtual _int			Update_Component(const _float& fTimeDelta);
	virtual void			LateUpdate_Component(void);

public:
	_bool		Check_Sphere_InterSect(_vec3 vCenter1 , _vec3 vCenter2,_float fRadius1, _float fRadius2);
	
	_bool		Check_Lay_InterSect(CRcTex * rcTex, CTransform* pMonsterCom, HWND hWnd);
	_bool		Check_Lay_InterSect(CRcTex * rcTex, CTransform* pMonsterCom, HWND hWnd, _vec3& PickPos);
	
	// 이동관련
	_bool		Check_Collision_Wall(CGameObject* pCubeObject, CGameObject* pDest );
	_bool		Check_Collision(CGameObject* pItemObject, CGameObject* pPlayer,const _float& fItemRadius,const _float& fPlayerRadius);
	_bool		Check_CollisonUseCollider(CCollider* pSour, CCollider* pDest);

	_bool		Check_Collsion_CubeAABB(CCollider* CWallCollider, CGameObject* pDest);

	
public:
	void		Set_HitBoxMatrix(_matrix* matWorld);
	void		Set_HitBoxMatrix_With_Scale(_matrix* matWorld,const _vec3& vScale);
	const	_matrix& HitBoxWolrdmat() { return m_HitBoxWolrdmat; }

	void		Set_HitRadiuos(_float fRadius) { m_fRadius = fRadius; }
	void		Set_vCenter(_vec3* vCenter ,_vec3* vScale);

	
	


private:
	_bool		Check_Collsion_AABB(CCollider* CWallCollider, CCollider* pDest);

public:
	virtual CComponent* Clone(void) override;
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev);






private:
	_matrix		m_HitBoxWolrdmat;

	bool		m_bClone;
	_vec3*		m_pPos;
	_vec3		m_vMin;
	_vec3		 m_vMax;
	_vec3		 m_vCenter;
	_float		 m_fRadius;
	
private:
	virtual void Free(void) override;
};

END