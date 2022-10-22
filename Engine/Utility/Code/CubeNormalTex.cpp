#include "..\..\Header\CubeNormalTex.h"
#include "Export_Function.h"

USING(Engine)

CCubeNormalTex::CCubeNormalTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_vPos(nullptr), m_bClone(false)
{
}

CCubeNormalTex::CCubeNormalTex(const CCubeNormalTex& rhs)
	: CVIBuffer(rhs), m_vPos(rhs.m_vPos), m_bClone(true)
{

}

CCubeNormalTex::~CCubeNormalTex()
{
}

HRESULT CCubeNormalTex::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_vPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = 12;

	m_dwVtxSize = sizeof(tagVertexCubeNormalTex);
	m_dwFVF = FVF_CUBE_NORMAL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	tagVertexCubeNormalTex*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	// 앞면
	pVertex[0].vPos = { -1.f, 1.f, -1.f };
	pVertex[0].vNormal = *D3DXVec3Normalize(&pVertex[0].vNormal, &pVertex[0].vPos)*-1;
	pVertex[0].vTexUV = pVertex[0].vPos;
	m_vPos[0] = pVertex[0].vPos;
	
	pVertex[1].vPos = { 1.f, 1.f, -1.f };
	pVertex[1].vNormal = *D3DXVec3Normalize(&pVertex[1].vNormal, &pVertex[1].vPos)*-1;
	pVertex[1].vTexUV = pVertex[1].vPos;
	m_vPos[1] = pVertex[1].vPos;

	pVertex[2].vPos = { 1.f, -1.f, -1.f };
	pVertex[2].vNormal = *D3DXVec3Normalize(&pVertex[2].vNormal, &pVertex[2].vPos)*-1;
	pVertex[2].vTexUV = pVertex[2].vPos;
	m_vPos[2] = pVertex[2].vPos;

	pVertex[3].vPos = { -1.f, -1.f, -1.f };
	pVertex[3].vNormal = *D3DXVec3Normalize(&pVertex[3].vNormal, &pVertex[3].vPos)*-1;
	pVertex[3].vTexUV = pVertex[3].vPos;
	m_vPos[3] = pVertex[3].vPos;

	// 뒷면
	pVertex[4].vPos = { -1.f, 1.f, 1.f };
	pVertex[4].vNormal =  *D3DXVec3Normalize(&pVertex[4].vNormal, &pVertex[4].vPos) *-1;
	pVertex[4].vTexUV = pVertex[4].vPos;
	m_vPos[4] = pVertex[4].vPos;

	pVertex[5].vPos = { 1.f, 1.f, 1.f };
	pVertex[5].vNormal = *D3DXVec3Normalize(&pVertex[5].vNormal, &pVertex[5].vPos)*-1;
	pVertex[5].vTexUV = pVertex[5].vPos;
	m_vPos[5] = pVertex[5].vPos;

	pVertex[6].vPos = { 1.f, -1.f, 1.f };
	pVertex[6].vNormal = *D3DXVec3Normalize(&pVertex[6].vNormal, &pVertex[6].vPos)*-1;
	pVertex[6].vTexUV = pVertex[6].vPos;
	m_vPos[6] = pVertex[6].vPos;

	pVertex[7].vPos = { -1.f, -1.f, 1.f };
	pVertex[7].vNormal = *D3DXVec3Normalize(&pVertex[7].vNormal, &pVertex[7].vPos) *-1;
	pVertex[7].vTexUV = pVertex[7].vPos;
	m_vPos[7] = pVertex[7].vPos;
	


	/*_vec3 vIndex0 = *D3DXVec3Normalize(&vIndex0, &pVertex[0].vPos);
	_vec3 vIndex1 = *D3DXVec3Normalize(&vIndex1, &pVertex[1].vPos);
	_vec3 vIndex2 = *D3DXVec3Normalize(&vIndex2, &pVertex[2].vPos);
	_vec3 vIndex3 = *D3DXVec3Normalize(&vIndex3, &pVertex[3].vPos);
	_vec3 vIndex4 = *D3DXVec3Normalize(&vIndex4, &pVertex[4].vPos);
	_vec3 vIndex5 = *D3DXVec3Normalize(&vIndex5, &pVertex[5].vPos);
	_vec3 vIndex6 = *D3DXVec3Normalize(&vIndex6, &pVertex[6].vPos);
	_vec3 vIndex7 = *D3DXVec3Normalize(&vIndex7, &pVertex[7].vPos);


	_vec3 vNormalRightIndex0 = *D3DXVec3Cross(&vNormalRightIndex0, &vIndex1, &vIndex5);
	_vec3 vNormalRightIndex1 = *D3DXVec3Cross(&vNormalRightIndex1, &vIndex1, &vIndex2);
	
	_vec3 vNormalRightIndex2 = *D3DXVec3Cross(&vNormalRightIndex2, &vIndex0, &vIndex3);
	_vec3 vNormalRightIndex3 = *D3DXVec3Cross(&vNormalRightIndex3, &vIndex4, &vIndex7);

	pVertex[0].vNormal = *D3DXVec3Cross(&pVertex[0].vNormal, &vIndex0, &vIndex1);
	pVertex[3].vNormal = *D3DXVec3Cross(&pVertex[3].vNormal, &vIndex2, &vIndex3);
	pVertex[1].vNormal = pVertex[0].vNormal + pVertex[3].vNormal+ vNormalRightIndex0 + vNormalRightIndex1;
	pVertex[2].vNormal = pVertex[0].vNormal + pVertex[3].vNormal+ vNormalRightIndex1;
	pVertex[3].vNormal += vNormalRightIndex3 + vNormalRightIndex2;
	pVertex[0].vNormal + vNormalRightIndex2;

	pVertex[5].vNormal = *D3DXVec3Cross(&pVertex[5].vNormal, &vIndex4, &vIndex5)+ vNormalRightIndex0;
	pVertex[7].vNormal = *D3DXVec3Cross(&pVertex[7].vNormal, &vIndex7, &vIndex4) + vNormalRightIndex3;
	pVertex[4].vNormal = pVertex[5].vNormal + pVertex[7].vNormal + vNormalRightIndex2;
	pVertex[6].vNormal = pVertex[5].vNormal + pVertex[7].vNormal + vNormalRightIndex

	D3DXVec3Normalize(&pVertex[0].vNormal, &pVertex[0].vNormal);
	D3DXVec3Normalize(&pVertex[1].vNormal, &pVertex[1].vNormal);
	D3DXVec3Normalize(&pVertex[2].vNormal, &pVertex[2].vNormal);
	D3DXVec3Normalize(&pVertex[3].vNormal, &pVertex[3].vNormal);
	D3DXVec3Normalize(&pVertex[4].vNormal, &pVertex[4].vNormal);
	D3DXVec3Normalize(&pVertex[5].vNormal, &pVertex[5].vNormal);
	D3DXVec3Normalize(&pVertex[6].vNormal, &pVertex[6].vNormal);
	D3DXVec3Normalize(&pVertex[7].vNormal, &pVertex[7].vNormal);*/



	m_pVB->Unlock();


	INDEX32*		pIndex = nullptr;

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

	return S_OK;

}

void CCubeNormalTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CCubeNormalTex * CCubeNormalTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeNormalTex*	pInstance = new CCubeNormalTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CCubeNormalTex::Clone(void)
{
	return new CCubeNormalTex(*this);
}

void CCubeNormalTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone) // 원본 컴포넌트를 삭제할 때 메모리 해제
	{
		Safe_Delete_Array(m_vPos);
	}
}
