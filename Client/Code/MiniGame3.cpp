#include "stdafx.h"
#include "..\Header\MiniGame3.h"

#include "Export_Function.h"
#include "Stage.h"
#include "ShopUI.h"

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


	return S_OK;
}

_int CMiniGame3::Update_Scene(const _float & fTimeDelta)
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
	// 브금 바꾸기
	Engine::PlaySoundW(L"Paradox.mp3", SOUND_BGM, 0.1f);

	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	return iResult;

}

void CMiniGame3::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CMiniGame3::Render_Scene(void)
{
}

HRESULT CMiniGame3::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CMiniGame3::Ready_Proto(void)
{
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
