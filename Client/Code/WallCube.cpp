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

	if ((*vMousPos).x == 0 && (*vMousPos).y == 0) {}
	else
	{
		MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수
	}

	_vec3 vCurretPos;
	m_pTransCom->Get_Info(INFO_POS, &vCurretPos);
	vCurretPos.y += 0.5f;
	m_pTransCom->Set_Y(vCurretPos.y);




	return S_OK;
}

HRESULT CWallCube::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	// set_hit_distance 
	m_pColliderCom->Set_HitRadiuos(1.f);
	m_pColliderCom->Set_HitRadiuos(1.f);
	m_pColliderCom->Set_vCenter(&vPos);


	
	return S_OK;
}

_int CWallCube::Update_Object(const _float & fTimeDelta)
{
	m_pColliderCom->Set_HitBoxMatrix(&(m_pTransCom->m_matWorld));

	if (Get_DIKeyState(DIK_M) & 0X80)
	{
		CScene  *pScene = ::Get_Scene();
		NULL_CHECK_RETURN(pScene, );
		CLayer * pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
		NULL_CHECK_RETURN(pLayer, );

		pLayer->Clear_Stack();

		pLayer->m_iRestRoom = 5;

		//vector<CGameObject*> temp = *pLayer->GetRestCube();

		random_shuffle((*pLayer->GetRestCube()).begin(), (*pLayer->GetRestCube()).end());

		pLayer->m_iRoomIndex = pLayer->m_iRestRoom--;
		CGameObject* pFirstCubeObj = (*pLayer->GetRestCube())[pLayer->m_iRoomIndex];
		
		

		pLayer = pScene->GetLayer(L"Layer_GameLogic");
		CGameObject* pPlayer = pLayer->Get_GameObject(L"Player");
		
		CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pPlayer->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
		CTransform* pFirstCubeTransform = dynamic_cast<CTransform*>(pFirstCubeObj->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		_vec3 vFirstCubePos;
		pFirstCubeTransform->Get_Info(INFO_POS, &vFirstCubePos);

		pTransform->Set_Pos(vFirstCubePos.x+5.f, vFirstCubePos.y, vFirstCubePos.z + 5.f);

		pTransform->Update_Component(1.f);


	}

	
	
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CWallCube::Render_Obejct(void)
{
	if (m_iOption == (_int)CUBE_START_TELE)
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
	else if (m_iOption == _int(CUBE_END_TELE))
	{
		
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pTextureCom->Set_Texture(m_iTexIndex);
		m_pBufferCom->Render_Buffer();
		// Hit Box 
		/*m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());

		m_pColliderCom->Render_Buffer();*/
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
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
		//m_pColliderCom->Check_Collision_Wall(this, pGameObject);
	}

	else if (m_iOption == (_int)CUBE_START_TELE)
	{
		// 스타트는 히트박스 그려주고있음
		CCollider *pCollider = dynamic_cast<CCollider*>(pGameObject->Get_Component(L"Proto_ColliderCom", ID_STATIC));

		if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
		{
				CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
				pLayer = pScene->GetLayer(L"Layer_CubeCollsion");

				CWallCube* pTeleCube = dynamic_cast<CWallCube*>(pLayer->Get_PreRoomTeleCube());

				if (pTeleCube == nullptr)
					return;

				CTransform* sour = dynamic_cast<CTransform*>(pTeleCube->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
				pLayer->m_iRestRoom++;
				_vec3 vPos;
				sour->Get_Info(INFO_POS, &vPos);

				pTransform->Set_Pos(vPos.x + 10.f, vPos.y, vPos.z+10.f);
				pTransform->Update_Component(1.f);
		}

	}

	else if (m_iOption == (_int)CUBE_END_TELE)
	{
		CCollider *pCollider = dynamic_cast<CCollider*>(pGameObject->Get_Component(L"Proto_ColliderCom", ID_STATIC));

		if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
		{
			CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

			pLayer = pScene->GetLayer(L"Layer_CubeCollsion");

			if (pLayer->m_iRestRoom < 0)
				return;

			vector<CGameObject*> temp = *pLayer->GetRestCube();
			_int iCurrentRoomIndex = pLayer->m_iRoomIndex;
			
			
			//pLayer->Delete_Current_Room(pLayer->m_iRoomIndex);

			_int iRandomNum =  pLayer->m_iRestRoom--; //rand() %
			if(pLayer->m_iRestRoom >= 0)
				pLayer->Save_CurrentRoom(this);
		 

			
			CTransform* sour = dynamic_cast<CTransform*>(temp[iRandomNum]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
			

			_vec3 vPos;
			sour->Get_Info(INFO_POS, &vPos);
			pTransform->Set_Pos(vPos.x+5.f, vPos.y, vPos.z+5.f);
			pTransform->Update_Component(1.f);
		
			return;

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

	// 툴쓸때 주석해야됌
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
