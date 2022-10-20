#include "Calculator.h"


#include "Export_Function.h"

USING(Engine)

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

Engine::CCalculator::CCalculator(const CCalculator& rhs)
	:CComponent(rhs)
{

}

CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator(void)
{

	return S_OK;
}

Engine::_float CCalculator::HeightOnTerrain(_vec3* pPos, const _vec3* pTerrainVtxPos, 
											const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	_float	fWidth = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fHeight = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE		Plane;

	
	if (fWidth > fHeight)
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);

	}

	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	pPos->y = (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;

	return pPos->y;
}

Engine::_vec3 Engine::CCalculator::PickingOnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom)
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
	
	pTerrainTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]], 
				&pTerrainVtx[dwVtxIdx[2]], 
				&vRayPos, &vRayDir, 
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU, 
							0.f,
							 pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}
			
			
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}
	
	return _vec3(0.f, 0.f, 0.f);
}

_vec3 CCalculator::PickingOnTerrainCube(HWND hWnd, const CTerrainTex * pTerrainBufferCom, const CTransform * pTerrainTransformCom)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// ����Ʈ -> ����
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;


	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;		

	vRayPos = { 0.f, 0.0f, 0.f };
	vRayDir = vPoint - vRayPos;

	

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	
	_matrix		matWorld ,matRevese;
	_vec3	Temp;
	pTerrainTransformCom->Get_WorldMatrix(&matWorld);

	D3DXMatrixInverse(&matRevese, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matRevese);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matRevese);


	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	// 로컬에서 말고 지금 로컬이니까 월드로 

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			_vec3 Normal = (pTerrainVtx[dwIndex + dwVtxCntX + 1] + pTerrainVtx[dwIndex])/2;


			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				D3DXVec3TransformCoord(&Normal, &Normal, &matWorld);
				return  Normal;
			}

			
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				D3DXVec3TransformCoord(&Normal, &Normal, &matWorld);
				return  Normal;
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

_bool  CCalculator::PickingOnTransform(HWND hWnd, const CCubeTex * pCubeTexBufferCom, const CTransform * pCubeTransCom)
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
	pCubeTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pCubtVtx = pCubeTexBufferCom->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;
	// +X
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 6;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 2;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~+X

	// -X
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 0;
	dwVtxIdx[2] = 3;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 3;
	dwVtxIdx[2] = 7;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~-X

	// Y
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 1;
	
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 0;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~ +Y

	// ~ -Y
	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 6;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 7;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~ -Y

	// Z+
	dwVtxIdx[0] = 7;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 5;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 7;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 4;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	// ~Z+

	// Z-
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

	// ~Z-


	return false;
}

_bool CCalculator::PickingOnTransform(HWND hWnd, const CCubeNormalTex * pCubeTexBufferCom, const CTransform * pCubeTransCom)
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
	pCubeTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pCubtVtx = pCubeTexBufferCom->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;
	// +X
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 6;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 2;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~+X

	// -X
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 0;
	dwVtxIdx[2] = 3;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 3;
	dwVtxIdx[2] = 7;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~-X

	// Y
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 1;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 0;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~ +Y

	// ~ -Y
	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 6;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 7;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~ -Y

	// Z+
	dwVtxIdx[0] = 7;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 5;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 7;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 4;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	// ~Z+

	// Z-
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

	// ~Z-


	return false;
}

_bool CCalculator::PickingOnTransform_Monster(HWND hWnd, const CRcTex * pMonsterTexBuffer, const CTransform * pMonsterTransCom)
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

	pMonsterTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pTerrainVtx = pMonsterTexBuffer->Get_VtxPos();

	_ulong		dwVtxCntX = 2;
	_ulong		dwVtxCntZ = 2;

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	/*for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;*/
	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 2;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return true;
			}

			dwVtxIdx[0] = 0;
			dwVtxIdx[1] = 2;
			dwVtxIdx[2] = 3;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return true;
			}

	return false;
}

_bool CCalculator::PickingTerrainObject(HWND hWnd, const CTerrainTex * pTerrainBufferCom, const CTransform* pTerrainTransformCom)
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

	pTerrainTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);


	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;


	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return true;
			}

			
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return true;
			}
		}
	}



	return false;
}

_bool CCalculator::PickingOnTransform_RcTex(HWND hWnd, const CRcTex* pRcTexCom, const CTransform* pTransform)
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

	vRayPos = { 0.f, 0.0f, 0.f };
	vRayDir = vPoint - vRayPos;



	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);


	_matrix		matWorld;
	pTransform->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);


	const _vec3*	pTerrainVtx = pRcTexCom->Get_VtxPos();


	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 2;

	if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[0]],
		&pTerrainVtx[dwVtxIdx[1]],
		&pTerrainVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 3;

	if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[0]],
		&pTerrainVtx[dwVtxIdx[1]],
		&pTerrainVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	return false;
}

_bool CCalculator::Picking_OrthoUI(HWND hWnd, const CRcTex * pRcTexCom, const CTransform* pUITransform)
{
	POINT		ptMouse{};

	//m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	//m_pTransCom->Set_Pos(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, 0.f);

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);


	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.1f;

	// 직교투영으로
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;

	vRayPos = { 0.f, 0.0f, 0.f };
	vRayDir = vPoint - vRayPos;
	

	
	

	const _vec3* vecUI = pRcTexCom->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 2;
		
		if (D3DXIntersectTri(&vecUI[dwVtxIdx[0]],
			&vecUI[dwVtxIdx[1]],
			&vecUI[dwVtxIdx[2]],
			&vRayPos, &vRayDir,
			&fU, &fV, &fDist))
		{
			MSG_BOX("충돌");
			return true;
		}


		dwVtxIdx[0] = 0;
		dwVtxIdx[1] = 2;
		dwVtxIdx[2] = 3;

		if (D3DXIntersectTri(&vecUI[dwVtxIdx[0]],
			&vecUI[dwVtxIdx[1]],
			&vecUI[dwVtxIdx[2]],
			&vRayPos, &vRayDir,
			&fU, &fV, &fDist))
		{
			MSG_BOX("충돌");
			return true;
		}

		return false;
	
}

_vec3 * CCalculator::Get_MouseProjAxis(HWND hWnd)
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(hWnd, &m_ptMouse);

	_vec3		vPoint;

	//D3DVIEWPORT9		ViewPort;
	//ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	//m_pGraphicDev->GetViewport(&ViewPort);
	
	vPoint.x = (_float)m_ptMouse.x; // / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = (_float)m_ptMouse.y; // / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.1f;
	
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	m_vecProjMouse = { vPoint.x, vPoint.y, vPoint.z };

	return &m_vecProjMouse;
}


CComponent* CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

CCalculator* CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		MSG_BOX("Calculator Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCalculator::Free(void)
{
	CComponent::Free();
}
