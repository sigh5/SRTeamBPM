#include "stdafx.h"
#include "ToolTest.h"
#include "Export_Function.h"

#include "TestCube.h"
#include "DynamicCamera.h"
#include "ImGuiMgr.h"

#include "Terrain.h"
#include "SkyBox.h"
#include "TestMonster.h"

#include "TestPlayer.h"



CToolTest::CToolTest(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CToolTest::~CToolTest()
{
	
}

HRESULT CToolTest::Ready_Scene()
{
	m_SceneType = SCENE_TOOLTEST;

	CImGuiMgr::GetInstance()->Ready_MapTool(m_pGraphicDev, this);
	CImGuiMgr::GetInstance()->Ready_MonsterTool(m_pGraphicDev, this);
	CImGuiMgr::GetInstance()->Ready_PlayerTool(m_pGraphicDev, this);

	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = m_pCam = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);


	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	// Tool Test Purpose's Player
	//pGameObject = CTestPlayer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);
	//

	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestMap", pGameObject), E_FAIL);

	m_mapLayer.insert({ L"TestLayer", pLayer });

	
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	

	return CScene::Ready_Scene();
}

_int CToolTest::Update_Scene(const _float& fTimeDelta)
{
	CImGuiMgr::GetInstance()->WindowLayOut();
	CImGuiMgr::GetInstance()->TerrainTool(m_pGraphicDev, this);
	CImGuiMgr::GetInstance()->CreateObject(m_pGraphicDev, this, m_pCam,L"TestCube");
	CImGuiMgr::GetInstance()->MonsterTool(m_pGraphicDev, this, m_pCam);

	// Tool Test Purpose's Player
//	CImGuiMgr::GetInstance()->Player_Tool(m_pGraphicDev, this, m_PpDirectory, L"TestLayer", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC);

	return CScene::Update_Scene(fTimeDelta);
}

void CToolTest::Free()
{
	CScene::Free();
}

CToolTest* CToolTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolTest*	pInstance = new CToolTest(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
