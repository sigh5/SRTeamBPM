#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"
#include "SkyBox.h"
#include "Effect.h"

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

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = 0;
	if (0 != m_MonsterList.size())
	{
		for (auto& iter : m_MonsterList)
		{
			iResult = iter->Update_Object(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;
		}
		if (Engine::Get_DIKeyState(DIK_P) & 0X80)
		{
			auto iter = m_MonsterList.front();

			CMonster* pObj = iter;

			Engine::Collect_Obj(static_cast<CGameObject*>(pObj));

			m_MonsterList.pop_front();
		}
	}
	else
	{


		if (Engine::Get_DIKeyState(DIK_L) & 0X80)
		{
			_vec3 vTestMonster1;

			vTestMonster1 = { 15.f, 0.f, 40.f };

			CGameObject* pObj = Engine::Reuse_Obj(m_pGraphicDev, vTestMonster1);

			m_MonsterList.push_back(static_cast<CMonster*>(pObj));
		}
	}

	if (0 != m_PotionList.size())
	{
		for (auto& iter : m_PotionList)
		{
			iResult = iter->Update_Object(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;
		}
	}
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
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
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// StaticCamera
	/*pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 20.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);*/

	// skybox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);


	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// testPlayer
	pGameObject = CTestPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);

	//몬스터 테스트용
	/*pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);*/

	m_mapLayer.insert({ pLayerTag, pLayer });

	Ready_Monster();

	Ready_HpPotion();

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	
	for (_uint i = 0; i < 50; ++i)
	{
		// testPlayer
		pGameObject = CEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);
	}



	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Monster(void)
{
	_vec3 vTestMonster1;

	vTestMonster1 = { 15.f, 0.f, 40.f };

	CMonster* pMonster = CMonster::Create(m_pGraphicDev, vTestMonster1);

	m_MonsterList.emplace_back(pMonster);

	/*_vec3 Monster1, Monster2, Monster3;

	Monster1 = { 15.f, 0.f, 40.f };

	Monster2 = { 17.f, 0.f, 45.f };

	Monster3 = { 19.f, 0.f, 50.f };

	m_MonsterPool->Reuse_Object(m_pGraphicDev, Monster1);
	m_MonsterPool->Reuse_Object(m_pGraphicDev, Monster2);
	m_MonsterPool->Reuse_Object(m_pGraphicDev, Monster3);*/

	return S_OK;
}

HRESULT CStage::Ready_HpPotion(void)
{
	_vec3 vTestHpPotion1;

	vTestHpPotion1 = { 10.f, 0.f, 15.f };

	CHpPotion* pHpPotion = CHpPotion::Create(m_pGraphicDev, vTestHpPotion1);

	m_PotionList.emplace_back(pHpPotion);

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
	for_each(m_MonsterList.begin(), m_MonsterList.end(), CDeleteObj());
	m_MonsterList.clear();

	for_each(m_PotionList.begin(), m_PotionList.end(), CDeleteObj());
	m_PotionList.clear();
	CScene::Free();

}

HRESULT CStage::Ready_Light(void)
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}
