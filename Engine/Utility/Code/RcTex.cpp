#include "..\..\Header\RcTex.h"
#include "Export_Function.h"

USING(Engine)

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_pPos(nullptr), m_bClone(false)
{
}


Engine::CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs), m_pPos(rhs.m_pPos), m_bClone(true)
{

}

CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Ready_Buffer(void)
{
	m_dwVtxCnt = 4;
	m_pPos = new _vec3[m_dwVtxCnt];
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
	pVertex[0].vPos = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = {0.f, 0.f};
	

	pVertex[1].vPos = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = {1.f, 0.f};

	pVertex[2].vPos = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = {1.f, 1.f};

	pVertex[3].vPos = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = {0.f, 1.f};

	m_pVB->Unlock();
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pPos[i] = pVertex[i].vPos;
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

void CRcTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CRcTex * CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex*	pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CRcTex::Clone(void)
{
	return new CRcTex(*this);
}

void CRcTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone) // 원본 컴포넌트를 삭제할 때 메모리 해제
	{
		Safe_Delete_Array(m_pPos);
	}
}
