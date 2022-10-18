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


// UI Test
#include "TestUI.h"


CMiniStage1::CMiniStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev), m_bFinish(false)
{
}

CMiniStage1::~CMiniStage1()
{
}

HRESULT CMiniStage1::Ready_Scene(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	

	return S_OK;

}

_int CMiniStage1::Update_Scene(const _float & fTimeDelta)
{
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
