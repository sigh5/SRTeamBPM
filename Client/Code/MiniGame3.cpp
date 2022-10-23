#include "stdafx.h"
#include "..\Header\MiniGame3.h"

#include "Export_Function.h"

#include "MiniStage3PreHeader.h"
#include "FileIOMgr.h"
#include "UI_Timer.h"
#include "Change_Stage.h"

CMiniGame3::CMiniGame3(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CMiniGame3::~CMiniGame3()
{
}

HRESULT CMiniGame3::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;
	m_SceneType = SCENE_MINISTAGE3;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(L"Layer_Monster"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_CubeMap(L"Layer_CubeMap"), E_FAIL);
	


	return S_OK;
}

_int CMiniGame3::Update_Scene(const _float & fTimeDelta)
{

	CLayer* pLayer = GetLayer(L"Layer_UI");
	CUI_Timer* pTimer = static_cast<CUI_Timer*>(pLayer->Get_GameObject(L"Timer"));

	if (pTimer->Get_Time() >= 10.f) //120초로 바꾸기
	{
		CScene*		pChangeScene = CChange_Stage::Create(m_pGraphicDev, 6);
		NULL_CHECK_RETURN(pChangeScene, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pChangeScene), -1);
		Engine::StopSound(SOUND_BGM);
		return -1;
	}



	// 브금 바꾸기
	Engine::PlaySoundW(L"Paradox.mp3", SOUND_BGM, 0.1f);

	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	return iResult;

}

void CMiniGame3::LateUpdate_Scene(void)
{

	CLayer *pLayer = GetLayer(L"Layer_GameLogic");

	for (auto iter = pLayer->Get_GameObjectMap().begin(); iter != pLayer->Get_GameObjectMap().end(); ++iter)
	{
		iter->second->Collision_Event();	
	}

	for (auto iter : pLayer->Get_ControlRoomList())
	{
		iter->Collision_Event();
	}
	for (auto iter : pLayer->Get_EffectList())
	{
		(iter)->Collision_Event();
	}


	pLayer = GetLayer(L"Layer_CubeMap");

	for (auto iter = pLayer->Get_GameObjectMap().begin(); iter != pLayer->Get_GameObjectMap().end(); ++iter)
	{
		iter->second->Collision_Event();
	}

	pLayer = GetLayer(L"Layer_Monster");
	
	pLayer->ClearActivevecListColliderMonster();

	for (auto iter : pLayer->Get_EffectList())
	{
		pLayer->Add_m_vecColiderMiniMonster(iter);
	}

	pLayer->ActivevecListColliderMonster();

	for (auto iter = pLayer->Get_EffectList().begin(); iter != pLayer->Get_EffectList().end(); ++iter)
	{
		(*iter)->Collision_Event();
	}



	Engine::CScene::LateUpdate_Scene();
}

void CMiniGame3::Render_Scene(void)
{
}

HRESULT CMiniGame3::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject  = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox2", pGameObject), E_FAIL);

	

	pGameObject = CStageTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniTerrain4", pGameObject), E_FAIL);

	


	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CMiniGame3::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CMiniPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;

}

HRESULT CMiniGame3::Ready_Layer_CubeMap(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev, this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"MiniStage2.dat",
		L"Wall",
		OBJ_CUBE);

	return S_OK;
}

HRESULT CMiniGame3::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	
	/*pGameObject = CMonsterMini::Create(m_pGraphicDev);*/
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);

	//pLayer->Add_GhulList(pGameObject);

	m_mapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CMiniGame3::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject*		pGameObject = nullptr;

	pGameObject = CUI_Timer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Timer", pGameObject), E_FAIL);




	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniGame3::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainMoveTexCom", CTerrainMoveTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniPlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/GuitarMan/GuitarMan%d.png", TEX_NORMAL, 22)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Bullet_Guita_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Bullet_Guita.png", TEX_NORMAL)), E_FAIL);

	return S_OK;
}

CMiniGame3 * CMiniGame3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniGame3*	pInstance = new CMiniGame3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniGame3::Free(void)
{
	Engine::CScene::Free();
}
