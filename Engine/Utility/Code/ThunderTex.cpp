#include "ThunderTex.h"



CThunderTex::CThunderTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_vPos(nullptr), m_bClone(false)
{
}

Engine::CThunderTex::CThunderTex(const CThunderTex& rhs)
	: CVIBuffer(rhs), m_vPos(rhs.m_vPos), m_bClone(true)
{

}

CThunderTex::~CThunderTex()
{
}

HRESULT CThunderTex::Ready_Buffer(void)
{
	m_dwVtxCnt = 4;
	m_vPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	// 오른쪽 위 삼각형
	pVertex[0].vPos = { -0.5f, 0.5f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };


	pVertex[1].vPos = { 0.5f, 0.5f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPos = { 0.5f, -0.5f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPos = { -0.5f, -0.5f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_vPos[i] = pVertex[i].vPos;
	}
	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CThunderTex::Render_Buffer(void)
{
}

CThunderTex * CThunderTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

CComponent * CThunderTex::Clone(void)
{
	return nullptr;
}

void CThunderTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone) // 원본 컴포넌트를 삭제할 때 메모리 해제
	{
		Safe_Delete_Array(m_vPos);
	}
}
