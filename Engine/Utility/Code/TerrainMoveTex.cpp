#include "..\..\Header\TerrainMoveTex.h"
#include "Export_Function.h"

USING(Engine)



CTerrainMoveTex::CTerrainMoveTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_hFile(nullptr), m_bClone(false), m_pPos(nullptr)
{
	ZeroMemory(&m_fH, sizeof(BITMAPFILEHEADER));
	ZeroMemory(&m_iH, sizeof(BITMAPINFOHEADER));
	ZeroMemory(&m_vCenter, sizeof(_vec3));
}

CTerrainMoveTex::CTerrainMoveTex(const CTerrainMoveTex & rhs)
	: CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_bClone(true)
	, m_pPos(rhs.m_pPos)
{
	memcpy(&m_fH, &rhs.m_fH, sizeof(BITMAPFILEHEADER));
	memcpy(&m_iH, &rhs.m_iH, sizeof(BITMAPINFOHEADER));
	memcpy(&m_vCenter, &rhs.m_vCenter, sizeof(_vec3));
}

CTerrainMoveTex::~CTerrainMoveTex()
{
}

HRESULT CTerrainMoveTex::Ready_Buffer(const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_pPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_ulong	dwByte = 0;

	m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NO_SCRUB_DATA, 0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[m_iH.biHeight * m_iH.biWidth];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, nullptr);

	CloseHandle(m_hFile);

	_ulong	dwIndex = 0;

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3���� : �迭�� ����� ù ��° ���ؽ��� �ּҸ� ����

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPos = { _float(j) * dwVtxItv,
				0,//( pPixel[dwIndex] & 0x000000ff) / 20.f,  
				_float(i) * dwVtxItv };

			m_pPos[dwIndex] = pVertex[dwIndex].vPos;
			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };

			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * 30.f,
				_float(i) / (dwCntZ - 1) * 30.f };
		}
	}

	m_vCenter = m_pPos[_int(m_dwVtxCnt*0.5)];
	Safe_Delete_Array(pPixel);

	_ulong dwTriCnt = 0;

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);


	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// ������ ��
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;

			_vec3	vDest, vSour, vNormal;

			vDest = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._0].vPos;
			vSour = pVertex[pIndex[dwTriCnt]._2].vPos - pVertex[pIndex[dwTriCnt]._1].vPos;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;
			++dwTriCnt;

			// ���� �Ʒ�
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;

			vDest = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._0].vPos;
			vSour = pVertex[pIndex[dwTriCnt]._2].vPos - pVertex[pIndex[dwTriCnt]._1].vPos;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;
			++dwTriCnt;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CTerrainMoveTex::Ready_Buffer(const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv, _float fTimeDelta)
{
	_ulong	dwIndex = 0;

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3���� : �迭�� ����� ù ��° ���ؽ��� �ּҸ� ����
	_int iHeight = (_int)fTimeDelta;
	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;
			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * 30.f,
				_float(i) / (dwCntZ - 1) * 30.f };
			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };
			float cosTime = 1.f*cosf(fTimeDelta *pVertex[dwIndex].vTexUV.x * fTimeDelta)*0.5f;
			pVertex[dwIndex].vPos = { _float(j) * dwVtxItv ,
				cosTime,//( pPixel[dwIndex] & 0x000000ff) / 20.f,  
				_float(i) * dwVtxItv };

			m_pPos[dwIndex] = pVertex[dwIndex].vPos;
		}
	}

	m_vCenter = m_pPos[_int(m_dwVtxCnt*0.5)];

	_ulong dwTriCnt = 0;

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);


	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// ������ ��
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;

			_vec3	vDest, vSour, vNormal;

			vDest = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._0].vPos;
			vSour = pVertex[pIndex[dwTriCnt]._2].vPos - pVertex[pIndex[dwTriCnt]._1].vPos;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;
			++dwTriCnt;

			// ���� �Ʒ�
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;

			vDest = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._0].vPos;
			vSour = pVertex[pIndex[dwTriCnt]._2].vPos - pVertex[pIndex[dwTriCnt]._1].vPos;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;
			++dwTriCnt;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CTerrainMoveTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CTerrainMoveTex * CTerrainMoveTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	CTerrainMoveTex*	pInstance = new CTerrainMoveTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTerrainMoveTex::Clone(void)
{
	return new CTerrainMoveTex(*this);
}

void CTerrainMoveTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone) // ���� ������Ʈ�� ������ �� �޸� ����
	{
		Safe_Delete_Array(m_pPos);
	}
}
