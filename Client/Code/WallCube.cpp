#include "stdafx.h"
#include "..\Header\WallCube.h"


#include "Export_Function.h"
#include "Terrain.h"
#include "AbstractFactory.h"
#include "SphinxFlyHead.h"


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
		MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수
	
	_vec3 vCurretPos;
	m_pTransCom->Get_Info(INFO_POS, &vCurretPos);
	vCurretPos.y += 0.5f;
	m_pTransCom->Set_Y(vCurretPos.y);




	m_pColliderCom->Set_HitRadiuos(5.0f);
	//m_pColliderCom->Set_vCenter(&vCurretPos ,);


	return S_OK;
}

HRESULT CWallCube::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_vec3 vPos,vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale =m_pTransCom->Get_Scale();
	// set_hit_distance 
	m_pColliderCom->Set_HitRadiuos(1.f);
	
	//if(m_iOption==4)
	//	m_pColliderCom->Set_vCenter(&vPos, &vScale);




	return S_OK;
}

_int CWallCube::Update_Object(const _float & fTimeDelta)
{
	if(m_iOption==4)
		m_pColliderCom->Set_HitBoxMatrix_With_Scale(&(m_pTransCom->m_matWorld),m_pTransCom->Get_Scale());

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CWallCube::Render_Obejct(void)
{
	/*if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);*/

	
	// Hit Box 
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_iOption == CUBE_COLLISION_WALL)
	{
		CScene* pScene = Get_Scene();
		if (pScene->Get_SceneType() == SCENE_TOOLTEST)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pTextureCom->Set_Texture(m_iTexIndex);
			m_pBufferCom->Render_Buffer();
			m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
			m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pColliderCom->Render_Buffer();
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		else
		{
			return;
		}
	}
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	
	if (m_bWireFrame)
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(150, 0, 0, 0));
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}

	m_pTextureCom->Set_Texture(m_iTexIndex);
	
	if (m_bWireFrame)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	}
	m_pBufferCom->Render_Buffer();

	//// HitBox
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//// ~Hit Box 
	
	if (m_bWireFrame)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}

void CWallCube::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");

	if (pScene->Get_SceneType() == SCENE_TOOLTEST)
		return;

	if (m_iOption == (_int)CUBE_COLLISION_WALL)
	{

		//CCollider* pColider = static_cast<CCollider*>(pGameObject->Get_Component(L"Proto_ColliderCom", ID_STATIC));

		if (m_pColliderCom->Check_Collsion_CubeAABB(m_pColliderCom, pGameObject))
			_bool b = false;
		
		for (auto iter : pLayer->Get_MapObject())
		{
			if (nullptr == dynamic_cast<CMonsterBase*>(iter.second))
			{
				continue;
			}
			else
			{
				if (nullptr != dynamic_cast<CSphinxFlyHead*>(iter.second))
					continue;
				if (m_pColliderCom->Check_Collsion_CubeAABB(m_pColliderCom, iter.second))
					_bool b = false;
			}
		}
		pLayer->Clear_ColliderMonster();

		
	}


}

void CWallCube::init_For_Collistion_vector()
{
	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	m_pColliderCom->Set_HitRadiuos(1.f);

	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	m_pColliderCom->Set_HitBoxMatrix(&(m_pTransCom->m_matWorld));
}

void CWallCube::MousePostoScreen()	// 지형타기
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TerrainTexCom", ID_STATIC));

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TransformCom", ID_DYNAMIC));

	if (pTerrainTransformCom == nullptr || pTerrainBufferCom == nullptr)
	{
		MSG_BOX("Choose Terrain");
		return;

	}

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
