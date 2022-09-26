#include "..\..\Header\UserInterface.h"
#include "Export_Function.h"

USING(Engine)

CUserInterface::CUserInterface()
{
}

CUserInterface::CUserInterface(const CUserInterface & rhs)
	: CComponent(rhs)
{
}

CUserInterface::~CUserInterface()
{
}

HRESULT CUserInterface::Ready_UserInterface(void)
{
	return S_OK;
}

_int CUserInterface::Update_Component(const _float & fTimeDelta)
{
	CComponent::Update_Component(fTimeDelta);
	return 0;
}

void CUserInterface::LateUpdate_Component(void)
{
	CComponent::LateUpdate_Component();
}


_matrix* CUserInterface::UpdateOrthogonal(_float fTextureSizeX, _float fTextureSizeY, _matrix* pMatrix)
{
	D3DXMatrixIdentity(pMatrix);

	m_pTransformCom->Get_WorldMatrix(pMatrix);
	D3DXMatrixIdentity(pMatrix);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, pMatrix);
	D3DXMatrixIdentity(pMatrix);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, pMatrix);
	
	_matrix		matOrtho;
	D3DXMatrixIdentity(&matOrtho);
	// 직교행렬 얻어옴
	m_pCameraCom->CalculateOrtho(&matOrtho);

	pMatrix = &matOrtho;

	_float fScale[AX_MAX];

	fScale[AX_X] = fTextureSizeX;
	fScale[AX_Y] = fTextureSizeY;
	fScale[AX_Z] = 1.f;

	for (_int i = 0; i < AX_MAX; ++i)
	{
		for (_int j = 0; j < 3; ++j)
			(i, j) *= fScale[i];
	}
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pMatrix);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, pMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, pMatrix);

	return pMatrix;
}

CUserInterface * CUserInterface::Create(void)
{
	CUserInterface* pInstance = new CUserInterface;

	if (FAILED(pInstance->Ready_UserInterface()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CUserInterface::Clone(void)
{
	return new CUserInterface(*this);
}

void CUserInterface::Free(void)
{
	CComponent::Free();
}
