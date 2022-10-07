#include "stdafx.h"
#include "..\Header\WallCube.h"


#include "Export_Function.h"
#include "Terrain.h"



CWallCube::CWallCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CWallCube::~CWallCube()
{
}

HRESULT CWallCube::InitSetting(_vec2* vMousPos, const wstring & LayerName,wstring* RoomName)
{

	Set_Layer_Map_Name(LayerName, RoomName);


	CScene* pScene = ::Get_Scene();

	CLayer* pLayer = pScene->GetLayer(LayerName.c_str());

	/*map<const _tchar*, CGameObject*> RoomMap = pLayer->Get_GameObjectMap();


	for (auto iter = RoomMap.begin(); iter != RoomMap.end(); ++iter)
	{
		if (true == static_cast<CTerrain*>(iter->second)->Set_SelectGizmo())
		{
			m_RoomName = iter->first;
		}
	}*/



	if ((*vMousPos).x == 0 && (*vMousPos).y == 0) {}
	else
	{
		MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수
	}

	_vec3 vCurretPos;
	m_pTransCom->Get_Info(INFO_POS, &vCurretPos);
	vCurretPos.y += 0.5f;
	m_pTransCom->Set_Y(vCurretPos.y);
								
	//_vec3	vScale = { 0.5f,0.5f,0.5f };
	//m_pTransCom->Set_Scale(&vScale);


	return S_OK;
}

HRESULT CWallCube::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
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
	if (m_iOption == 2 || m_iOption ==3 )
	{
		if (Get_DIKeyState(DIK_C) & 0X80)
		{
			return;
		}
	}


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
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );





	_vec3 Temp = m_pCalculatorCom->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	//_vec3 vPos,vAngle,RealPos, vScale;
	//pTerrainTransformCom->Get_Info(INFO_POS, &vPos);
	//vScale = pTerrainTransformCom->Get_Scale();
	//vAngle = pTerrainTransformCom->Get_Angle();

	//RealPos = { vPos.x + Temp.x *vScale.x, vPos.y + Temp.y*vScale.y, vPos.z + Temp.z*vScale.x };

	//_matrix matRot;
	//D3DXMatrixIdentity(&matRot);

	////D3DXMatrixIdentity(&matRot);
	//D3DXMatrixRotationY(&matRot, vAngle.y);



	//D3DXVec3TransformNormal(&RealPos, &RealPos, &matRot);


	

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);

	

	

}

_bool CWallCube::Set_SelectGizmo()
{
	return m_pCalculatorCom->PickingOnTransform(g_hWnd, m_pBufferCom, m_pTransCom);
}

HRESULT CWallCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MapCubeTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MapCubeTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });




	return S_OK;
}

CWallCube * CWallCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWallCube*	pInstance = new CWallCube(pGraphicDev);


	if (FAILED(pInstance->Ready_Object()))
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
