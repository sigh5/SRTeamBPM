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

_bool CCollider::Check_Lay_InterSect(CRcTex * rcTex, CTransform* pMonsterCom, HWND hWnd)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);


	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vPoint - vRayPos;


	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix		matWorld;

	pMonsterCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pCubtVtx = rcTex->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;
	// 오른쪽 위
	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 2;
	
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// 왼쪽 아래
	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 3;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}



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
