#include "stdafx.h"
#include "..\Header\MiniStage1.h"

#include "Export_Function.h"
#include "TestPlayer.h"
#include "TestMonster.h"
#include "Stage.h"
#include "ToolTest.h"
#include "Stage_Pjw.h"
#include "Change_Stage.h"
#include "Setting_Stage.h"
#include "ShopUI.h"



CMiniStage1::CMiniStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev), m_bFinish(false)
{
}

CMiniStage1::~CMiniStage1()
{
}

HRESULT CMiniStage1::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	m_SceneType = SCENE_MINISTAGE1;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);


	return S_OK;

}

_int CMiniStage1::Update_Scene(const _float & fTimeDelta)
{
	if (Key_Down(DIK_Q))
	{
		CScene* pStage1 = ::Get_SaveScene();
		CLayer* pLayer = pStage1->GetLayer(L"Layer_GameLogic");
		CShopUI* pShopUI = dynamic_cast<CShopUI*>(pLayer->Get_GameObject(L"ShopUI"));
		pShopUI->Set_ForceScene(0);
		pStage1->Set_SceneChane(false);

		Load_SaveScene(this);
		return -1;
	}
	Engine::PlaySoundW(L"Paradox.mp3", SOUND_BGM, 0.1f);

	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	return iResult;
}

void CMiniStage1::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CMiniStage1::Render_Scene(void)
{
}

HRESULT CMiniStage1::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CMiniStage1::Ready_Proto(void)
{
	return S_OK;
}

CMiniStage1 * CMiniStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniStage1*	pInstance = new CMiniStage1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniStage1::Free(void)
{
	Engine::CScene::Free();
}
