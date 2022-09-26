#include "..\..\Header\OrthoTransform.h"

#include "Export_Function.h"

USING(Engine)

COrthoTransform::COrthoTransform()
	:CTransform()	
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matOrtho);
}

COrthoTransform::COrthoTransform(const COrthoTransform & rhs)
	: CTransform(rhs)
{
	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(m_vInfo[i], rhs.m_vInfo[i], sizeof(_vec3));

	memcpy(m_matOrtho, rhs.m_matOrtho, sizeof(_matrix));
}


COrthoTransform::~COrthoTransform()
{
}

HRESULT COrthoTransform::Ready_OrthoTransform(void)
{
	return S_OK;
}

_int COrthoTransform::Update_Component(const _float & fTimeDelta)
{

	CTransform::Update_Component(fTimeDelta);
	
	return _int();
}

void COrthoTransform::OrthoMatrix(	_float fScaleX, 
									_float fScaleY,
									_float fPosX,
									_float fPoxY,
									 _float fWidth, _float fHeight)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matWorld);

	m_matView.m[0][0] = fScaleX;
	m_matView.m[1][1] = fScaleY;
	m_matView.m[2][2] = 1.f;

	m_matView.m[3][0] = fPosX;
	m_matView.m[3][1] = fPoxY;

	_float fFar = 1.f;
	_float fNear = 0.f;

	D3DXMatrixOrthoLH(&m_matOrtho, fWidth, fHeight, fNear, fFar);
}



COrthoTransform * COrthoTransform::Create(void)
{
	COrthoTransform* pInstance = new COrthoTransform;
	if (FAILED(pInstance->Ready_OrthoTransform()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * COrthoTransform::Clone(void)
{
	return new COrthoTransform(*this);
}

void COrthoTransform::Free(void)
{
	CTransform::Free();
}
