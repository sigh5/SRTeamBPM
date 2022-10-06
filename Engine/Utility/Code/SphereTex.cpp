#include "..\..\Header\SphereTex.h"
#include "Export_Function.h"

USING(Engine)

static WORD sphere_vertex(UINT slices, int slice, int stack)
{
	return stack * slices + slice + 1;
}

CSphereTex::CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


Engine::CSphereTex::CSphereTex(const CSphereTex& rhs)
	: CVIBuffer(rhs)
{

}

CSphereTex::~CSphereTex()
{
}

HRESULT CSphereTex::Ready_Buffer(void)
{
	int number_of_vertices, number_of_faces;
	float radius = 0.5f;
	int slices = 20;
	int stacks = 20;
	float phi_step, phi_start;
	float theta_step, theta, sin_theta, cos_theta;
	int vertex, face;
	int slice, stack;
	number_of_vertices = 2 + slices * (stacks - 1);
	number_of_faces = 2 * slices + (stacks - 2) * (2 * slices);

	m_dwVtxCnt = number_of_vertices;
	m_dwTriCnt = number_of_faces;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_pGraphicDev->CreateVertexBuffer(
		sizeof(VTXTEX)*number_of_vertices,
		D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		NULL);

	m_pGraphicDev->CreateIndexBuffer(sizeof(int) * number_of_faces * 3,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pIB,
		NULL);

	VTXTEX *vertices;
	HRESULT hRes = m_pVB->Lock(0, 0, (void**)&vertices, 0);


	WORD *faces;
	HRESULT hRes2 = m_pIB->Lock(0, 0, (void**)&faces, 0);

	if (FAILED(hRes2)) {
		return E_FAIL;
	}

	phi_step = -2 * D3DX_PI / slices;
	phi_start = D3DX_PI / 2;

	theta_step = D3DX_PI / stacks;
	theta = theta_step;

	vertex = 0;
	face = 0;
	stack = 0;

	vertices[vertex].vPos.x = 0.0f;
	vertices[vertex].vPos.y = 0.0f;
	vertices[vertex].vPos.z = radius;
	vertices[vertex].vTexUV = _vec2(0.9f, 0.9f);

	float deltaU = 1.f / static_cast<float>(slices);
	float deltaV = 1.f / static_cast<float>(stacks);

	vertex++;

	for (stack = 0; stack < stacks - 1; stack++) {
		sin_theta = sinf(theta);
		cos_theta = cosf(theta);

		for (slice = 0; slice < slices; slice++) {
			vertices[vertex].vNormal.x = sin_theta * cosf(phi_start);
			vertices[vertex].vNormal.y = sin_theta * sinf(phi_start);
			vertices[vertex].vNormal.z = cos_theta;
			vertices[vertex].vPos.x = radius * sin_theta * cosf(phi_start);
			vertices[vertex].vPos.y = radius * sin_theta * sinf(phi_start);
			vertices[vertex].vPos.z = radius * cos_theta;
			vertices[vertex].vTexUV = _vec2(deltaU * slice * 2, deltaV * (stack + 1) * 2);
			vertex++;

			phi_start += phi_step;

			if (slice > 0) {
				if (stack == 0) {
					faces[face++] = 0;
					faces[face++] = slice + 1;
					faces[face++] = slice;
				}
				else {
					faces[face++] = sphere_vertex(slices, slice - 1, stack - 1);
					faces[face++] = sphere_vertex(slices, slice, stack - 1);
					faces[face++] = sphere_vertex(slices, slice - 1, stack);

					faces[face++] = sphere_vertex(slices, slice, stack - 1);
					faces[face++] = sphere_vertex(slices, slice, stack);
					faces[face++] = sphere_vertex(slices, slice - 1, stack);
				}
			}
		}

		theta += theta_step;

		if (stack == 0) {
			faces[face++] = 0;
			faces[face++] = 1;
			faces[face++] = slice;
		}
		else {
			faces[face++] = sphere_vertex(slices, slice - 1, stack - 1);
			faces[face++] = sphere_vertex(slices, 0, stack - 1);
			faces[face++] = sphere_vertex(slices, slice - 1, stack);

			faces[face++] = sphere_vertex(slices, 0, stack - 1);
			faces[face++] = sphere_vertex(slices, 0, stack);
			faces[face++] = sphere_vertex(slices, slice - 1, stack);
		}
	}

	vertices[vertex].vPos.x = 0.0f;
	vertices[vertex].vPos.y = 0.0f;
	vertices[vertex].vPos.z = -radius;
	vertices[vertex].vTexUV = _vec2(0.1f, 1.f);
	vertices[vertex].vNormal.x = 0.0f;
	vertices[vertex].vNormal.y = 0.0f;
	vertices[vertex].vNormal.z = -1.0f;

	for (slice = 1; slice < slices; slice++) {
		faces[face++] = sphere_vertex(slices, slice - 1, stack - 1);
		faces[face++] = sphere_vertex(slices, slice, stack - 1);
		faces[face++] = vertex;
	}

	faces[face++] = sphere_vertex(slices, slice - 1, stack - 1);
	faces[face++] = sphere_vertex(slices, 0, stack - 1);
	faces[face++] = vertex;
	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CSphereTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CSphereTex * CSphereTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereTex*   pInstance = new CSphereTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSphereTex::Clone(void)
{
	return new CSphereTex(*this);
}

void CSphereTex::Free(void)
{
	CVIBuffer::Free();
}