#include "..\..\Header\UserInterface.h"
#include "Export_Function.h"

USING(Engine)

CUserInterface::CUserInterface(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUserInterface::~CUserInterface()
{
}

HRESULT CUserInterface::Ready_Object(void)
{
	/*D3DXMatrixIdentity(&m_matProj);
	
	_matrix		matTest;
	D3DXMatrixIdentity(&matTest);

	m_pCameraCom->Get_ProjMatrix(&matTest);
	
	m_matProj = matTest;*/

	//_matrix		matWorld;
	//D3DXMatrixIdentity(&matWorld);

	//// World Matrix
	//m_pTransformCom->Get_WorldMatrix(&matWorld);
	//
	//// View Matrix
	//_matrix		matView;
	//D3DXMatrixIdentity(&matView);

	//m_pCameraCom->Get_ViewMatrix(&matView);
	//D3DXMatrixIdentity(&matView);

	UpdateOrthogonal(150.f, 150.f);


	return S_OK;
}

_int CUserInterface::Update_Object(const _float & fTimeDelta)
{
	UpdateOrthogonal(150.f, 150.f);


	return _int();
}

void CUserInterface::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CUserInterface::Render_Obejct(void)
{
	CGameObject::Render_Obejct();
}

void CUserInterface::UpdateOrthogonal(_float fTextureSizeX, _float fTextureSizeY)
{
	_matrix		matWorld, matView, matProj;
	
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	_matrix		matOrtho;
	D3DXMatrixIdentity(&matOrtho);
	// 직교행렬 얻어옴
	m_pCameraCom->CalculateOrtho(&matOrtho);

	matProj = matOrtho;

	_float fScale[WINCMAX];

	fScale[WINCX] = fTextureSizeX;
	fScale[WINCY] = fTextureSizeY;
	fScale[WINCZ] = 1.f;

	for (_int i = 0; i < WINCMAX; ++i)
	{
		for (_int j = 0; j < 3; ++j)
			matView(i, j) *= fScale[i];
	}
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

}

void CUserInterface::Free(void)
{
	CGameObject::Free();
}
