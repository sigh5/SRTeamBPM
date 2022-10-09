#include "stdafx.h"
#include "..\Header\WallCube.h"


#include "Export_Function.h"
#include "Terrain.h"
#include "AbstractFactory.h"


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
	

	// set_hit_distance 
	m_pColliderCom->Set_HitRadiuos(1.5f);

	return S_OK;
}

_int CWallCube::Update_Object(const _float & fTimeDelta)
{



	m_pColliderCom->Set_HitBoxMatrix(&(m_pTransCom->m_matWorld));

	CGameObject::Update_Object(fTimeDelta);

	
	 

	Add_RenderGroup(RENDER_PRIORITY, this);



	return 0;

}

void CWallCube::Render_Obejct(void)
{
	if (m_iOption == 2 || m_iOption ==3 )
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pTextureCom->Set_Texture(m_iTexIndex);
		m_pBufferCom->Render_Buffer();
		// Hit Box 
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pColliderCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		// ~Hit Box 
		return;
	}

	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	// Hit Box 
	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	// ~Hit Box 
	
	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	

}

void CWallCube::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");

	if (m_iOption == (_int)CUBE_WALL)
	{
		m_pColliderCom->Check_Collision_Wall(this, pGameObject);
	}

	else if (m_iOption == (_int)CUBE_START_TELE)
	{

		CCollider *pCollider = dynamic_cast<CCollider*>(pGameObject->Get_Component(L"Proto_ColliderCom", ID_STATIC));

		if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
		{
			CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
			_vec3 vPos = { 100.f,0.f,100.f };
			pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
		}

	}

	else if (m_iOption == (_int)CUBE_END_TELE)
	{
		CCollider *pCollider = dynamic_cast<CCollider*>(pGameObject->Get_Component(L"Proto_ColliderCom", ID_STATIC));

		if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
		{
			CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
			_vec3 vPos = { 100.f,0.f,100.f };
			pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}


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


	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

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
