#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"
#include "SkyBox.h"
#include "Effect.h"

#include "FileIOMgr.h"
#include "ObjectMgr.h"

#include "Player.h"

#include "MyCamera.h"

#include "Box.h"
#include "HealthPotion.h"

#include "HpBar.h"
#include "CoinKeyUI.h"
#include "Status_UI.h"
#include "MetronomeUI.h"

#include "Snowfall.h"
#include "HitBlood.h"

#include "Anubis.h"
#include "Spider.h"



CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	m_SceneType = SCENE_STAGE;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_CubeCollsion(L"Layer_CubeCollsion"), E_FAIL);
	
	::PlaySoundW(L"SamTow.wav", SOUND_BGM, 0.05f); // BGM

	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	
	m_fFrame += 1 * fTimeDelta;

	if (m_fFrame >= 1.f)
	{

		CGameObject*		pGameObject = nullptr;
		CLayer *pMyLayer = GetLayer(L"Layer_UI");

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, -WINCX / 2.f, 0.f, +150.f, 1);
		NULL_CHECK_RETURN(pGameObject, 0);
		pMyLayer->Add_GameObjectList(pGameObject);

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, WINCX / 2.f, 0, -150.f, 0);
		NULL_CHECK_RETURN(pGameObject, 0);
		pMyLayer->Add_GameObjectList(pGameObject);

		m_fFrame = 0.f;
	}

	//CLayer *pLayer1 = GetLayer(L"Layer_GameLogic");
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(pLayer1->Get_GameObject(L"Player"));

	//CLayer * pLayer2 = GetLayer(L"Layer_CubeCollsion");

	//pLayer2->Get_GameObjectMap();

	//for (auto iter = pLayer2->Get_GameObjectMap().begin(); iter != pLayer2->Get_GameObjectMap().end(); ++iter)
	//{
	//	pPlayer->Collsion_CubeMap(iter->second, fTimeDelta);
	//}


	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
	
	/*CAnubis* pAbubis = dynamic_cast<CAnubis*>(pLayer->Get_GameObject(L"TestMonster1"));

	if (pPlayer->Get_CheckShot() == true)
	{
		pAbubis->Collision_Event(pPlayer);
	}*/

	CLayer *pLayer1 = GetLayer(L"Layer_GameLogic");

	for (auto iter = pLayer1->Get_GameObjectMap().begin(); iter != pLayer1->Get_GameObjectMap().end(); ++iter)
	{
		iter->second->Collision_Event();
	}


	Engine::CScene::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{

}

HRESULT CStage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CMyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMyCamera", pGameObject), E_FAIL);
	
	
	READY_LAYER(pGameObject, CSkyBox, pLayer, m_pGraphicDev, L"SkyBox");
	READY_LAYER(pGameObject, CTerrain, pLayer, m_pGraphicDev, L"Terrain");
	READY_LAYER(pGameObject, CSnowfall, pLayer, m_pGraphicDev, L"Snowfall");
	


	/*pGameObject = CHitBlood::Create(m_pGraphicDev, _vec3{ 3.f, 1.f, 3.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"hitblood", pGameObject), E_FAIL);*/



	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });
	
	CGameObject*		pGameObject = nullptr;

	READY_LAYER(pGameObject, CPlayer, pLayer, m_pGraphicDev, L"Player");

	/*CPlayer*		pPlayer = dynamic_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
	pGameObject = CBox::Create(m_pGraphicDev, 5, 10, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box", pGameObject), E_FAIL);
*/


	//READY_LAYER(pGameObject, CAnubis, pLayer, m_pGraphicDev, L"TestMonster1");



	/*CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
		this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Monster.dat",
		L"Monster",
		OBJ_MONSTER);*/

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));

	pGameObject = CBullet_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet_UI", pGameObject), E_FAIL);

	pGameObject = CWeapon_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Weapon_UI", pGameObject), E_FAIL);

	pGameObject = CHpBar::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HpBar", pGameObject), E_FAIL);

	pGameObject = CStatus_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Status_UI", pGameObject), E_FAIL);

	pGameObject = CCoinKeyUI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CoinKey_UI", pGameObject), E_FAIL);



	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_CubeCollsion(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
		this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Stage1Map.dat",
		L"TestCube",
		OBJ_CUBE);

	return S_OK;
}

HRESULT CStage::Ready_Proto(void)
{
	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage::Free(void)
{
	CScene::Free();
}

HRESULT CStage::Ready_Light(void)
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type		= D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction  = _vec3(0.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}
