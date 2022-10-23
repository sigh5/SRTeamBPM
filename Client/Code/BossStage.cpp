#include "stdafx.h"
#include "..\Header\BossStage.h"

#include "Export_Function.h"

#include "Stage1PreHeader.h"


CBossStage::CBossStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;
	

	m_SceneType = SCENE_STAGE;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	/*FAILED_CHECK_RETURN(Ready_Layer_CubeCollsion(L"Layer_CubeCollsion"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Room(L"Layer_Room"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Icon(L"Layer_Icon"), E_FAIL);*/
	
	return S_OK;
}

_int CBossStage::Update_Scene(const _float & fTimeDelta)
{

	m_fFrame += 1.f * fTimeDelta;

	//if (m_fFrame >= 1.f)
	//{
	//	CGameObject*		pGameObject = nullptr;
	//	CLayer *pMyLayer = GetLayer(L"Layer_UI");

	//	pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, -WINCX / 2.f - 400.f, 0.f, +150.f, 1);
	//	NULL_CHECK_RETURN(pGameObject, 0);
	//	pMyLayer->Add_GameObjectList(pGameObject);

	//	pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, WINCX / 2.f + 400.f, 0, -150.f, 0);
	//	NULL_CHECK_RETURN(pGameObject, 0);
	//	pMyLayer->Add_GameObjectList(pGameObject);

	//	pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeSmallUI(m_pGraphicDev, -WINCX / 2.f - 370.f, 0.f, +150.f, 3);
	//	NULL_CHECK_RETURN(pGameObject, 0);
	//	pMyLayer->Add_GameObjectList(pGameObject);

	//	pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeSmallUI(m_pGraphicDev, WINCX / 2.f + 370.f, 0, -150.f, 2);
	//	NULL_CHECK_RETURN(pGameObject, 0);
	//	pMyLayer->Add_GameObjectList(pGameObject);

	//	for (auto iter = pMyLayer->Get_GhulList().begin(); iter != pMyLayer->Get_GhulList().end(); ++iter)
	//	{
	//		(*iter)->Update_Object(fTimeDelta);
	//	}
	//	m_fFrame = 0.f;
	//}


	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CBossStage::LateUpdate_Scene(void)
{
	//CLayer *pLayer = GetLayer(L"Layer_GameLogic");

	//for (auto iter = pLayer->Get_GameObjectMap().begin(); iter != pLayer->Get_GameObjectMap().end(); ++iter)
	//{
	//	if (nullptr != dynamic_cast<CMonsterBase*>(iter->second))
	//	{
	//		if (false == dynamic_cast<CMonsterBase*>(iter->second)->Get_Dead())
	//			iter->second->Collision_Event();
	//	}
	//	else
	//	{
	//		iter->second->Collision_Event();
	//	}
	//}

	//for (auto iter = pLayer->Get_GhulList().begin(); iter != pLayer->Get_GhulList().end(); ++iter)
	//{
	//	if (false == static_cast<CMonsterBase*>(*iter)->Get_Dead())
	//	{
	//		(*iter)->LateUpdate_Object();
	//		(*iter)->Collision_Event();
	//	}
	//}

	//for (auto iter : (pLayer->Get_DropItemList()))
	//	iter->Collision_Event();

	//pLayer = GetLayer(L"Layer_CubeCollsion");

	//for (auto iter = pLayer->Get_GameObjectMap().begin(); iter != pLayer->Get_GameObjectMap().end(); ++iter)
	//{
	//	iter->second->Collision_Event();
	//}

	//for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
	//{
	//	for (auto iter : *(pLayer->Get_TeleCubeList(i)))
	//		iter->Collision_Event();
	//}

	//Engine::CScene::LateUpdate_Scene();


	//pLayer = GetLayer(L"Layer_Room");
	//for (auto iter : (pLayer->Get_ControlRoomList()))
	//	iter->Collision_Event();

	//CPlayer_Dead_UI* pPlayerDead = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	//if (pPlayerDead->Get_Render() && !m_bStopBGM)
	//{
	//	Engine::StopSound(SOUND_BGM);
	//	m_bStopBGM = true;

	//	_uint iC = 0;
	//}

	//else
	//{
	//	Engine::PlaySoundW(L"SamTow.wav", SOUND_BGM, g_fSound); // BGM
	//	m_bStopBGM = false;
	//}

}

void CBossStage::Render_Scene(void)
{
}

void CBossStage::Copy_Scene_Data(CScene * pStage1Scene)
{

	/*_CubeCollsion(L"Layer_CubeCollsion"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Room(L"Layer_Room"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Icon(L"Layer_Icon"), E_FAIL); 
*/
	//CLayer** pBossLayer = nullptr;
	
	CLayer* pLayer = pStage1Scene->GetLayer(L"Layer_Environment");
	m_mapLayer.insert({ L"Layer_Environment", pLayer });

	pLayer = pStage1Scene->GetLayer(L"Layer_UI");
	m_mapLayer.insert({ L"Layer_UI", pLayer });

	pLayer = pStage1Scene->GetLayer(L"Layer_Icon");
	m_mapLayer.insert({ L"Layer_Icon", pLayer });

	pLayer = pStage1Scene->GetLayer(L"Layer_GameLogic");
	m_mapLayer.insert({ L"Layer_GameLogic", pLayer });


	//pBossLayer = GetLayer(L"Layer_UI");
	//pLayer = pStage1Scene->GetLayer(L"Layer_UI");	

	//for (auto iter : pLayer->Get_GameObjectMap())
	//{
	//	pBossLayer->Add_GameObject(iter.first, iter.second);
	//}
	//
	//pBossLayer = GetLayer(L"Layer_Icon");
	//pLayer = pStage1Scene->GetLayer(L"Layer_Icon");
	//for (auto iter : pLayer->Get_GameObjectMap())
	//{
	//	pBossLayer->Add_GameObject(iter.first, iter.second);
	//}

	//

	//Safe_Release(pStage1Scene);	 // 기존 scene을 삭제
	//Engine::Clear_RenderGroup();
	//
}

HRESULT CBossStage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	//// CDynamicCamera  CMyCamera
	//pGameObject = CMyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMyCamera", pGameObject), E_FAIL);


	//READY_LAYER(pGameObject, CSkyBox, pLayer, m_pGraphicDev, L"SkyBox");
	//READY_LAYER(pGameObject, CSnowfall, pLayer, m_pGraphicDev, L"Snowfall");

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CBossStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });
	//CGameObject*		pGameObject = nullptr;
	//READY_LAYER(pGameObject, CPlayer, pLayer, m_pGraphicDev, L"Player");



	return S_OK;
}

HRESULT CBossStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });




	return S_OK;
}

HRESULT CBossStage::Ready_Layer_Icon(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CBossStage::Ready_Layer_CubeCollsion(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	// 맵

	return S_OK;
}

HRESULT CBossStage::Ready_Layer_Room(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	// Terrain , 오브젝트

	return S_OK;
}

HRESULT CBossStage::Ready_Proto(void)
{
	return S_OK;
}

HRESULT CBossStage::Ready_Light(void)
{
	return S_OK;
}

CBossStage * CBossStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossStage *	pInstance = new CBossStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBossStage::Free(void)
{
	CScene::Free();
}
