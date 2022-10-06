#include "SphereTex.h"

USING(Engine)

CSphereTex::CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CSphereTex::CSphereTex(const CSphereTex& rhs)
	: CVIBuffer(rhs) , m_pIndexVB(rhs.m_pIndexVB) ,m_pMesh(rhs.m_pMesh)
{

}

CSphereTex::~CSphereTex()
{
}

HRESULT CSphereTex::Ready_Buffer(void)
{
	int Slices = 6;
	int stack = 4;
	int numVertex = (Slices + 1) * 2 + ((stack - 3)*Slices);
	int numface = (Slices * 2) + ((stack - 2) * 2) *Slices;

	D3DXCreateSphere(m_pGraphicDev, 2.0f, 5, 4, &m_pMesh, NULL);
	float Radius = 2.0f;
	Sphere_tex* g_vertices = new Sphere_tex[numVertex];
	g_vertices[0].vCenter.x = 0.0f;
	g_vertices[0].vCenter.y = Radius;
	g_vertices[0].vCenter.z = 0.f;
	g_vertices[0].iColor = 0xffffffff;
	for (int i = 0 + 1, k = 1; i < stack; ++i)
	{
		_vec3 temp (0.f, Radius*cosf((D3DX_PI / stack)*i), Radius * sinf((D3DX_PI / stack) *i));
		
		for (int j = 0; j < Slices; ++j, ++k)
		{
			float tmp = Radius - (Radius - temp.z);

		}
	}


	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	pVertex[0].vPos = { 0.f, 1.f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPos = { 1.f, -1.f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[2].vPos = { -1.f, -1.f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
}

void CSphereTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CSphereTex* CSphereTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereTex*	pInstance = new CSphereTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* Engine::CSphereTex::Clone(void)
{
	return new CSphereTex(*this);
}

void CSphereTex::Free(void)
{
	CVIBuffer::Free();
}

