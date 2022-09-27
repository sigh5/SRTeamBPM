#pragma once
#include "Component.h"


BEGIN(Engine)

class CGameObject;


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

public:
	_bool		Check_Sphere_InterSect(_vec3 vCenter1 , _vec3 vCenter2,_float fRadius1, _float fRadius2);


public:
	virtual CComponent* Clone(void) override;
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	bool		m_bClone;


private:
	virtual void Free(void) override;
};

END