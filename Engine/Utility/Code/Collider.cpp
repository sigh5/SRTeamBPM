#include "..\..\Header\Collider.h"


#include "Export_Function.h"

USING(Engine)


CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_bClone(false)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs), m_bClone(true)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_Collider(void)
{
	return S_OK;
}

_int CCollider::Update_Component(const _float & fTimeDelta)
{
	CComponent::Update_Component(fTimeDelta);

	return 0;
}

void CCollider::LateUpdate_Component(void)
{
	CComponent::LateUpdate_Component();
}

_bool CCollider::Check_Sphere_InterSect(_vec3 vCenter1, _vec3 vCenter2, _float fRadius1, _float fRadius2)
{
	_float fDistance; 
	_vec3 vDiff;
	vDiff = vCenter2 - vCenter1;
	fDistance = D3DXVec3Length(&vDiff);

	if (fDistance <= (fRadius1 + fRadius2))
		return true;

	return false;
}

CComponent * CCollider::Clone(void)
{
	return new CCollider(*this);
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider()))
	{
		MSG_BOX("Collider Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCollider::Free(void)
{
	CComponent::Free();
}
