#include "..\..\Header\Collider.h"


#include "Export_Function.h"

USING(Engine)


CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_bClone(false),m_pPos(nullptr), m_fRadius(0.f)
{
	D3DXMatrixIdentity(&m_HitBoxWolrdmat);

	ZeroMemory(m_vMin, sizeof(_vec3));
	ZeroMemory(m_vMax, sizeof(_vec3));
	ZeroMemory(m_vCenter, sizeof(_vec3));
	
	
	 
}

CCollider::CCollider(const CCollider & rhs)
	: CVIBuffer(rhs), m_bClone(true) ,m_pPos(rhs.m_pPos),m_fRadius(rhs.m_fRadius)
{
	memcpy(&m_vMin, &rhs.m_vMin, sizeof(_vec3));
	memcpy(&m_vMax, &rhs.m_vMax, sizeof(_vec3));
	memcpy(&m_vCenter, &rhs.m_vCenter, sizeof(_vec3));

}

HRESULT CCollider::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_pPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = 12;
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE*      pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	// 앞면
	pVertex[0].vPos = { -1.f, 1.f, -1.f };
	pVertex[0].vTexUV = pVertex[0].vPos;
	m_pPos[0] = pVertex[0].vPos;

	pVertex[1].vPos = { 1.f, 1.f, -1.f };
	pVertex[1].vTexUV = pVertex[1].vPos;
	m_pPos[1] = pVertex[1].vPos;

	pVertex[2].vPos = { 1.f, -1.f, -1.f };
	pVertex[2].vTexUV = pVertex[2].vPos;
	m_pPos[2] = pVertex[2].vPos;

	pVertex[3].vPos = { -1.f, -1.f, -1.f };
	pVertex[3].vTexUV = pVertex[3].vPos;
	m_pPos[3] = pVertex[3].vPos;

	// 뒷면
	pVertex[4].vPos = { -1.f, 1.f, 1.f };
	pVertex[4].vTexUV = pVertex[4].vPos;
	m_pPos[4] = pVertex[4].vPos;

	pVertex[5].vPos = { 1.f, 1.f, 1.f };
	pVertex[5].vTexUV = pVertex[5].vPos;
	m_pPos[5] = pVertex[5].vPos;

	pVertex[6].vPos = { 1.f, -1.f, 1.f };
	pVertex[6].vTexUV = pVertex[6].vPos;
	m_pPos[6] = pVertex[6].vPos;

	pVertex[7].vPos = { -1.f, -1.f, 1.f };
	pVertex[7].vTexUV = pVertex[7].vPos;
	m_pPos[7] = pVertex[7].vPos;

	//pVertex[0].vPos = { -2.f, 2.f, -2.f };
	//pVertex[0].vTexUV = pVertex[0].vPos;
	//m_pPos[0] = pVertex[0].vPos;

	//pVertex[1].vPos = { 2.f, 2.f, -2.f };
	//pVertex[1].vTexUV = pVertex[1].vPos;
	//m_pPos[1] = pVertex[1].vPos;

	//pVertex[2].vPos = { 2.f, -2.f, -2.f };
	//pVertex[2].vTexUV = pVertex[2].vPos;
	//m_pPos[2] = pVertex[2].vPos;

	//pVertex[3].vPos = { -2.f, -2.f, -2.f };
	//pVertex[3].vTexUV = pVertex[3].vPos;
	//m_pPos[3] = pVertex[3].vPos;

	//// 뒷면
	//pVertex[4].vPos = { -2.f, 2.f, 2.f };
	//pVertex[4].vTexUV = pVertex[4].vPos;
	//m_pPos[4] = pVertex[4].vPos;

	//pVertex[5].vPos = { 2.f, 2.f, 2.f };
	//pVertex[5].vTexUV = pVertex[5].vPos;
	//m_pPos[5] = pVertex[5].vPos;

	//pVertex[6].vPos = { 2.f, -2.f, 2.f };
	//pVertex[6].vTexUV = pVertex[6].vPos;
	//m_pPos[6] = pVertex[6].vPos;

	//pVertex[7].vPos = { -2.f, -2.f, 2.f };
	//pVertex[7].vTexUV = pVertex[7].vPos;
	//m_pPos[7] = pVertex[7].vPos;


	m_pVB->Unlock();


	INDEX32*      pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+

	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// Z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// Z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();


	m_vMin = pVertex[3].vPos;
	m_vMax = pVertex[5].vPos;
	m_vCenter = { m_vMin.x + 1.f, m_vMin.y + 1.f, m_vMin.z + 1.f };
	m_fRadius = 1.f;

	return S_OK;

}

void CCollider::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CCollider::~CCollider()
{
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

_bool CCollider::Check_Lay_InterSect(CRcTex * rcTex, CTransform* pMonsterCom, HWND hWnd, _vec3& PickPos)
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
		_vec3 v0, v1, v2;
		v0 = pCubtVtx[dwVtxIdx[1]];
		v1 = pCubtVtx[dwVtxIdx[0]];
		v2 = pCubtVtx[dwVtxIdx[2]];
		
		PickPos = v0 + fU*(v1 - v0) + fV * (v2 - v0);

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
		_vec3 v0, v1, v2;
		v0 = pCubtVtx[dwVtxIdx[1]];
		v1 = pCubtVtx[dwVtxIdx[0]];
		v2 = pCubtVtx[dwVtxIdx[2]];

		PickPos = v0 + fU*(v1 - v0) + fV * (v2 - v0);

		return true;
	}



	return false;
}

_bool CCollider::Check_Collision_Wall(CGameObject* pCubeObject, CGameObject* pDest)
{
	CTransform *pTrnasform = dynamic_cast<CTransform*>(pCubeObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	CDynamic_Transform* pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(pDest->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	_vec3 vCenter1Pos;
	_vec3 vPos;

	pTrnasform->Get_Info(INFO_POS, &vCenter1Pos);
	pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	if (Check_Sphere_InterSect(vCenter1Pos, vPos, 4.f, 1.f))
	{
		_vec3 vDirection, vUp, vPos,vRight;

		pDynamicTransCom->Get_Info(INFO_LOOK, &vDirection);
		pDynamicTransCom->Get_Info(INFO_UP, &vUp);
		pDynamicTransCom->Get_Info(INFO_POS, &vPos);

		pDynamicTransCom->Move_Pos(&pDynamicTransCom->Get_CounterMovePos());

		pDynamicTransCom->Update_Component(1.f);

		return true;
	}

	return false;

}

_bool CCollider::Check_Collision(CGameObject * pItemObject, CGameObject * pPlayer, const _float& fItemRadius, const _float& fPlayerRadius)
{
	CTransform *pTrnasform = dynamic_cast<CTransform*>(pItemObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	CDynamic_Transform* pDynamicTransCom = dynamic_cast<CDynamic_Transform*>(pPlayer->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	_vec3 vItemPos;
	_vec3 vPos;

	pTrnasform->Get_Info(INFO_POS, &vItemPos);
	pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	if (Check_Sphere_InterSect(vItemPos, vPos, fItemRadius, fPlayerRadius))
	{
		return true;
	}

	return false;
}

_bool CCollider::Check_CollisonUseCollider(CCollider * pSour, CCollider * pDest)
{
	if (Check_Sphere_InterSect(pSour->m_vCenter, pDest->m_vCenter, pSour->m_fRadius, pDest->m_fRadius))
		return true;
	
	return false;
}

void CCollider::Set_HitBoxMatrix(_matrix* matWorld)
{
	_matrix		matScale, matRotX, matRotY, matRotZ, matTrans;

	_vec3	vPos;
	
	memcpy(&vPos, &(matWorld->_41), sizeof(_vec3));

	D3DXMatrixScaling(&matScale, m_fRadius, m_fRadius, m_fRadius);
	D3DXMatrixRotationX(&matRotX, 0.f);
	D3DXMatrixRotationY(&matRotY, 0.f);
	D3DXMatrixRotationZ(&matRotZ, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	m_HitBoxWolrdmat = matScale * matRotX * matRotY * matRotZ * matTrans;


	
	memcpy(&m_vCenter, &m_HitBoxWolrdmat._41, sizeof(_vec3));
	
	


}

CComponent * CCollider::Clone(void)
{
	return new CCollider(*this);
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("Collider Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCollider::Free(void)
{

	if (false == m_bClone)
		Safe_Delete(m_pPos);


	CVIBuffer::Free();
}
