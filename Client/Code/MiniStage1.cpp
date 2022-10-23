#include "stdafx.h"
#include "..\Header\MiniStage1.h"

#include "Export_Function.h"


#include "Stage.h"
#include "ToolTest.h"

#include "Change_Stage.h"
#include "Setting_Stage.h"
#include "ShopUI.h"
#include "WrongPicFind.h"
#include "MiniGame1Pic.h"



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

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);

	Engine::PlaySoundW(L"Wrong_Image_Find_Game.mp3", SOUND_BGM, g_fSound);

	return S_OK;

}

_int CMiniStage1::Update_Scene(const _float & fTimeDelta)
{
	CWrongPicFind*	pWrongPicFind = static_cast<CWrongPicFind*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"WrongPicFind"));
	
	if (pWrongPicFind->Get_Success()) //(Key_Down(DIK_Q))
	{
		++m_iCount;

		if (m_iCount > 30)
		{			
			CScene* pStage1 = ::Get_SaveScene();
			CLayer* pMyLayer = pStage1->GetLayer(L"Layer_Icon");
			CMiniGame1Pic* pMinigamePic = dynamic_cast<CMiniGame1Pic*>(pMyLayer->Get_GameObject(L"Minigame1"));
			pMinigamePic->Set_GameClear(false);
			
			CScene* pChange = CChange_Stage::Create(m_pGraphicDev, 7);
			NULL_CHECK_RETURN(pChange, -1);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pChange), -1);
				

			return -1;
		}		
		//	Load_SaveScene(this);
	}

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
	Engine::CLayer*		pMyLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pMyLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CWrongPicFind::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(L"WrongPicFind", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pMyLayer });

	return S_OK;
}

HRESULT CMiniStage1::Ready_Proto(void)
{
	// FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Chim_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Chim_Binpole.png", TEX_NORMAL)), E_FAIL);

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
