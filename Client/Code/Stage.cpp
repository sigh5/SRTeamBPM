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
#include "ShotGun.h"

#include "HpBar.h"
#include "CoinKeyUI.h"
#include "Status_UI.h"
#include "MetronomeUI.h"
#include "Gun_Screen.h"
#include "DashUI.h"
#include "Inventory_UI.h"
#include "UI_Frame.h"

#include "Snowfall.h"
#include "HitBlood.h"

#include "Anubis.h"
#include "Spider.h"
#include "FatBat.h"
#include "Sphinx.h"
#include "Ghul.h"
#include "Obelisk.h"
#include "EarthShaker.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	//지우셈
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

	FAILED_CHECK_RETURN(Ready_Layer_CubeCollsion(L"Layer_CubeCollsion"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Room(L"Layer_Room"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Icon(L"Layer_Icon"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_CubeCollsion(L"Layer_CubeCollsion"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Room(L"Layer_Room"), E_FAIL);
	

	Set_Player_StartCubePos();

	//::PlaySoundW(L"SamTow.wav", SOUND_BGM, 0.05f); // BGM

	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	
	m_fFrame += 1.f * fTimeDelta;

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

		for (auto iter = pMyLayer->Get_GhulList().begin(); iter != pMyLayer->Get_GhulList().end(); ++iter)
		{
			(*iter)->Update_Object(fTimeDelta);
		}
		m_fFrame = 0.f;
	}

	TeleportCubeUpdate(fTimeDelta);

	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{

	CLayer *pLayer = GetLayer(L"Layer_GameLogic");

	for (auto iter = pLayer->Get_GameObjectMap().begin(); iter != pLayer->Get_GameObjectMap().end(); ++iter)
	{
		iter->second->Collision_Event();
	}
	
	for (auto iter = pLayer->Get_GhulList().begin(); iter != pLayer->Get_GhulList().end(); ++iter)
	{
		(*iter)->LateUpdate_Object();
		(*iter)->Collision_Event();
	}


	pLayer = GetLayer(L"Layer_CubeCollsion");

	for (auto iter = pLayer->Get_GameObjectMap().begin(); iter != pLayer->Get_GameObjectMap().end(); ++iter)
	{
		iter->second->Collision_Event();
	}

	for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
	{
		for (auto iter : *(pLayer->Get_TeleCubeList(i)))
			iter->Collision_Event();
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

	// CDynamicCamera  CMyCamera
	pGameObject = CMyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMyCamera", pGameObject), E_FAIL);
	
	
	READY_LAYER(pGameObject, CSkyBox, pLayer, m_pGraphicDev, L"SkyBox");
	//READY_LAYER(pGameObject, CTerrain, pLayer, m_pGraphicDev, L"Terrain");
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

	//pGameObject = CBox::Create(m_pGraphicDev, 20, 20);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box", pGameObject), E_FAIL);

	pGameObject = CSphinx::Create(m_pGraphicDev, 30, 39);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Sphinx", pGameObject), E_FAIL);

	pGameObject = CEarthShaker::Create(m_pGraphicDev, 20, 20, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Earth", pGameObject), E_FAIL);

	/*pGameObject = CObelisk::Create(m_pGraphicDev, 20, 15);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Obelisk", pGameObject), E_FAIL);*/

	//pLayer->Add_ObeliskList(pGameObject); //오벨리스크 생성 시 리스트에도 추가해야 함

	//pGameObject = CGhul::Create(m_pGraphicDev, 20, 15);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ghul", pGameObject), E_FAIL);

	/*CPlayer*		pPlayer = dynamic_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));
	pGameObject = CBox::Create(m_pGraphicDev, 5, 10, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box", pGameObject), E_FAIL);
*/

	// Gun_Screen 테스트 용으로 넣어놨음 참고
	pGameObject = CShotGun::Create(m_pGraphicDev, 10, 10);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShotGun", pGameObject), E_FAIL);


	//READY_LAYER(pGameObject, CAnubis, pLayer, m_pGraphicDev, L"TestMonster1");
	//READY_LAYER(pGameObject, CSpider, pLayer, m_pGraphicDev, L"TestMonster2");
	//READY_LAYER(pGameObject, CFatBat, pLayer, m_pGraphicDev, L"TestMonster3");

	//
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
	
	pGameObject = CStatus_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Status_UI", pGameObject), E_FAIL);

	pGameObject = CInventory_UI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"InventoryUI", pGameObject), E_FAIL);

	READY_LAYER(pGameObject, CGun_Screen, pLayer, m_pGraphicDev, L"Gun");

	READY_LAYER(pGameObject, CUI_Frame, pLayer, m_pGraphicDev, L"Frame");

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CStage::Ready_Layer_Icon(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	CTestPlayer*		pPlayer = dynamic_cast<CTestPlayer*>(Get_GameObject(L"Layer_GameLogic", L"TestPlayer"));

	pGameObject = CBullet_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet_UI", pGameObject), E_FAIL);

	pGameObject = CWeapon_UI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Weapon_UI", pGameObject), E_FAIL);

	pGameObject = CHpBar::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HpBar", pGameObject), E_FAIL);

	pGameObject = CCoinKeyUI::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CoinKey_UI", pGameObject), E_FAIL);

	pGameObject = CDashUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DashUI", pGameObject), E_FAIL);

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

	pLayer->initStartCube();

	return S_OK;
}

HRESULT CStage::Ready_Layer_Room(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
		this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Stage1Room.dat",
		L"Room",
		OBJ_ROOM);

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

void CStage::TeleportCubeUpdate(const _float & fTimeDelta)
{
	CLayer* pLayer = GetLayer(L"Layer_CubeCollsion");

	for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
	{
		for (auto iter :  *(pLayer->Get_TeleCubeList(i)))
			iter->Update_Object(fTimeDelta);
	}
}

void CStage::Set_Player_StartCubePos()
{
	CLayer* pLayer =GetLayer(L"Layer_CubeCollsion");
	pLayer->m_iRoomIndex = pLayer->m_iRestRoom--;  // rand() %
	vector<CGameObject*> m_vecCube = *pLayer->GetRestCube();
	CGameObject* pFirstCubeObj = m_vecCube[pLayer->m_iRoomIndex];

	CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(Get_Component(L"Layer_GameLogic",L"Player",L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CTransform* pFirstCubeTransform = dynamic_cast<CTransform*>(pFirstCubeObj->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	
	//pLayer->Save_CurrentRoom(pLayer->m_iRoomIndex);
	
	_vec3 vFirstCubePos;
	pFirstCubeTransform->Get_Info(INFO_POS, &vFirstCubePos);

	pTransform->Set_Pos(vFirstCubePos.x, vFirstCubePos.y, vFirstCubePos.z + 5.f);
	
	pTransform->Update_Component(1.f);
}
