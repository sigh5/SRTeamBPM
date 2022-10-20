#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"

#include "Stage1PreHeader.h"
#include "SkillParticle.h"
#include "FireWorks.h"
#include "PetYeti.h"
#include "EquipYeti.h"

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

	FAILED_CHECK_RETURN(Ready_Layer_CubeCollsion(L"Layer_CubeCollsion"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Room(L"Layer_Room"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Icon(L"Layer_Icon"), E_FAIL);


	Set_Player_StartCubePos();

	Engine::PlaySoundW(L"SamTow.wav", SOUND_BGM, g_fSound); // BGM

	CMouseMgr::GetInstance()->Mouse_Change(m_pGraphicDev, CUSOR_SHOT);
	
	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{

	m_fFrame += 1.f * fTimeDelta;

	if (m_fFrame >= 1.f )
	{
		CGameObject*		pGameObject = nullptr;
		CLayer *pMyLayer = GetLayer(L"Layer_UI");

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, -WINCX / 2.f - 400.f, 0.f, +150.f, 1);
		NULL_CHECK_RETURN(pGameObject, 0);
		pMyLayer->Add_GameObjectList(pGameObject);

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeUI(m_pGraphicDev, WINCX / 2.f + 400.f, 0, -150.f, 0);
		NULL_CHECK_RETURN(pGameObject, 0);
		pMyLayer->Add_GameObjectList(pGameObject);
	
		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeSmallUI(m_pGraphicDev, -WINCX / 2.f - 370.f, 0.f, +150.f, 3);
		NULL_CHECK_RETURN(pGameObject, 0);
		pMyLayer->Add_GameObjectList(pGameObject);

		pGameObject = CObjectMgr::GetInstance()->Reuse_MetronomeSmallUI(m_pGraphicDev, WINCX / 2.f + 370.f, 0, -150.f, 2);
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
		if (nullptr != dynamic_cast<CMonsterBase*>(iter->second))
		{
			if(false == dynamic_cast<CMonsterBase*>(iter->second)->Get_Dead())
			iter->second->Collision_Event();
		}
		else
		{
			iter->second->Collision_Event();
		}
	}
	
	for (auto iter = pLayer->Get_GhulList().begin(); iter != pLayer->Get_GhulList().end(); ++iter)
	{
		if (false == static_cast<CMonsterBase*>(*iter)->Get_Dead())
		{
			(*iter)->LateUpdate_Object();
			(*iter)->Collision_Event();
		}
	}

	for (auto iter : (pLayer->Get_DropItemList()))
		iter->Collision_Event();

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
	pLayer = GetLayer(L"Layer_Room");

	
	for (auto iter : (pLayer->Get_ControlRoomList()))
		iter->Collision_Event();

	CPlayer_Dead_UI* pPlayerDead = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	if (pPlayerDead->Get_Render() && !m_bStopBGM)
	{
		Engine::StopSound(SOUND_BGM);
		m_bStopBGM = true;

		_uint iC = 0;
	}
	
	else
	{
		Engine::PlaySoundW(L"SamTow.wav", SOUND_BGM, g_fSound); // BGM
		m_bStopBGM = false;
	}
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
	READY_LAYER(pGameObject, CSnowfall, pLayer, m_pGraphicDev, L"Snowfall");

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

	pGameObject = CBox::Create(m_pGraphicDev, 327, 315);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box0", pGameObject), E_FAIL);
	
	pGameObject = CBox::Create(m_pGraphicDev, 327, 320);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box1", pGameObject), E_FAIL);
	
	pGameObject = CBox::Create(m_pGraphicDev, 327, 325);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box2", pGameObject), E_FAIL);
	
	pGameObject = CBox::Create(m_pGraphicDev, 327, 330);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box3", pGameObject), E_FAIL);
	
	pGameObject = CNpc::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC", pGameObject), E_FAIL);
	
	READY_LAYER(pGameObject, CQuestNpc, pLayer, m_pGraphicDev, L"QuestNPC");


	pGameObject = CFireTrap::Create(m_pGraphicDev, 320, 320);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Trap1", pGameObject), E_FAIL);

	READY_LAYER(pGameObject, CShopUI, pLayer, m_pGraphicDev, L"ShopUI");

	pGameObject = CMagnum::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Magnum", pGameObject), E_FAIL);

	pGameObject = CHelmet::Create(m_pGraphicDev, 310, 325);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Helmet1", pGameObject), E_FAIL);

	pGameObject = CHelmet2::Create(m_pGraphicDev, 310, 340);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Helmet2", pGameObject), E_FAIL);

	pGameObject = CSkillParticle::Create(m_pGraphicDev, _vec3(330.f, 2.f, 330.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"testparticle1", pGameObject), E_FAIL);

	pGameObject = CPetYeti::Create(m_pGraphicDev, 330.f, 330.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Yeti", pGameObject), E_FAIL);

	pGameObject = CEquipYeti::Create(m_pGraphicDev, 310, 345);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EquipYeti", pGameObject), E_FAIL);

	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev,
		this,
		const_cast<_tchar*>(pLayerTag),
		L"../../Data/",
		L"Monster1.dat",
		L"Monster",
		OBJ_MONSTER);

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
	READY_LAYER(pGameObject, CAx, pLayer, m_pGraphicDev, L"AX");
	READY_LAYER(pGameObject, CUI_Effect, pLayer, m_pGraphicDev, L"Dash_Effect");
	READY_LAYER(pGameObject, CUI_Frame, pLayer, m_pGraphicDev, L"Frame");

	READY_LAYER(pGameObject, CQuestTalkingFrame, pLayer, m_pGraphicDev, L"QuestUIFrame");


	pGameObject = CPlayer_Dead_UI::Create(m_pGraphicDev, 255);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dead_UI", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CStage::Ready_Layer_Icon(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	CPlayer*		pPlayer = dynamic_cast<CPlayer*>(Get_GameObject(L"Layer_GameLogic", L"Player"));

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

	CFileIOMgr::GetInstance()->Load_FileData(m_pGraphicDev, this, const_cast<_tchar*>(pLayerTag),
		L"../../Data/", L"Stage1Obstacle.dat",
		L"Obstacle", OBJ_OBSTRACLE);



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
	/*D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type		= D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo.Direction  = _vec3(1.f, 0.f, -1.f);
	tLightInfo.Position = _vec3(100.f, 10.f, 100.f);
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	D3DLIGHT9		tLightInfo1;
	ZeroMemory(&tLightInfo1, sizeof(D3DLIGHT9));

	tLightInfo1.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo1.Diffuse =	 D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo1.Specular =	 D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo1.Ambient =	 D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo1.Direction = _vec3(-1.f, 0.f, 1.f);
	tLightInfo1.Position = _vec3(100.f, 10.f, 100.f);
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo1, 1), E_FAIL);


	D3DLIGHT9		tLightInfo2;
	ZeroMemory(&tLightInfo2, sizeof(D3DLIGHT9));

	tLightInfo2.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo2.Diffuse =	D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo2.Specular =	D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo2.Ambient =	D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	tLightInfo2.Direction = _vec3(0.f, 0.f, 1.f);
	tLightInfo2.Position = _vec3(200.f, 10.f, 200.f);
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo2, 2), E_FAIL);
*/



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
	
	_int iRandomShuffleNum = rand()%4;

	
	for(int i=0; i<iRandomShuffleNum; ++i)
		random_shuffle((*pLayer->GetRestCube()).begin(), (*pLayer->GetRestCube()).end());
	
	
	
	vector<CGameObject*> m_vecCube = *pLayer->GetRestCube();
	CGameObject* pFirstCubeObj = m_vecCube[pLayer->m_iRoomIndex];

	CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(Get_Component(L"Layer_GameLogic",L"Player",L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CTransform* pFirstCubeTransform = dynamic_cast<CTransform*>(pFirstCubeObj->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	
	//pLayer->Save_CurrentRoom(pLayer->m_iRoomIndex);
	_vec3 vAngle;
	_vec3 vFirstCubePos;
	pFirstCubeTransform->Get_Info(INFO_POS, &vFirstCubePos);
	vAngle = pFirstCubeTransform->Get_Angle();

	pTransform->Set_Pos(vFirstCubePos.x, vFirstCubePos.y, vFirstCubePos.z );
	pTransform->Rotation(ROT_Y, vAngle.y);
	pTransform->Update_Component(1.f);
}
