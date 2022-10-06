#include "stdafx.h"
#include "..\Header\WallCube.h"


#include "Export_Function.h"
#include "AbstractFactory.h"

CWallCube::CWallCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CWallCube::~CWallCube()
{
}

HRESULT CWallCube::Ready_Object(_vec2 *vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	if ((*vPos).x == 0 && (*vPos).y == 0) {}
	else
	{
		MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수
	}

	_vec3 vCurretPos;
	m_pTransCom->Get_Info(INFO_POS, &vCurretPos);
	vCurretPos.y += 0.5f;
	m_pTransCom->Set_Y(vCurretPos.y);
								
	_vec3	vScale = { 0.5f,0.5f,0.5f };
	m_pTransCom->Set_Scale(&vScale);

	return S_OK;
}

_int CWallCube::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);
	return 0;

}

void CWallCube::Render_Obejct(void)
{
	/*if (m_iOption == (_int)CUBE_TELEPORT)
		return;*/
	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();

	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CWallCube::MousePostoScreen()	// 지형타기
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );

	_vec3 Temp = m_pCalculatorCom->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);
}

_bool CWallCube::Set_SelectGizmo()
{
	return m_pCalculatorCom->PickingOnTransform(g_hWnd, m_pBufferCom, m_pTransCom);
}

HRESULT CWallCube::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MapCubeTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CWallCube * CWallCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 *vPos)
{
	CWallCube*	pInstance = new CWallCube(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWallCube::Free()
{
	CGameObject::Free();
}
