#include "stdafx.h"
#include "TestCube.h"

#include "Export_Function.h"



CTestCube::CTestCube(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CTestCube::CTestCube(const CGameObject& rhs) : CGameObject(rhs)
{
}

HRESULT CTestCube::Ready_Object(int Posx, int Posy)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (Posx == 0 && Posy == 0){}
	else
	{
		Set_TransformPositon();
	}
	_vec3 vScale = { 0.5f,0.5f,0.5f };
	m_pTransCom->Set_Scale(&vScale);
	return S_OK;
}

_int CTestCube::Update_Object(const _float& fTimeDelta)
{

	
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);
	return 0;
}

void CTestCube::Render_Obejct()
{
	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();

	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CTestCube::Set_TransformPositon()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom,);

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );


	_vec3 Temp = m_pCalculatorCom->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

}

bool CTestCube::Set_SelectGizmo()
{

	if (m_pCalculatorCom->PickingOnTransform(g_hWnd, m_pBufferCom, m_pTransCom))
		return true;


	return false;

}

HRESULT CTestCube::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubeTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
 	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });


	//pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_TerrainTexture3"));
	//NULL_CHECK_RETURN(m_pTextureCom2, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture3", pComponent });



	return S_OK;
}

CTestCube* CTestCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CTestCube*	pInstance = new CTestCube(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

