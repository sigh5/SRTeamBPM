#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_Function.h"

#include "Stage.h"
#include "ToolTest.h"

#include "Change_Stage.h"
#include "Setting_Stage.h"
#include "MiniGame3.h"

#include"EndingScene.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev), m_bFinish(false)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;
	
	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);
											// LOADING_STAGE  LOADING_TOOL ,LOADING_MINISTAGE3
	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	
	Engine::LoadSoundFile();
								// Paradox
	

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	++m_iCount;

	Engine::PlaySoundW(L"Paradox.mp3", SOUND_BGM, g_fSound);
	//cout << "�׽�Ʈ : " << m_iCount << endl;

	m_pStartButton = dynamic_cast<CStart_Button*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"StartButton"));
	m_pExitButton = dynamic_cast<CExit_Button*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"ExitButton"));
	m_pSettingButton = dynamic_cast<CSettingButton*>(Engine::Get_GameObject(L"Ready_Layer_Environment", L"SettingButton"));

	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	if (m_pLoading->Get_Finish())
	{
		if (m_pStartButton->Get_Click())
		{						// CChange_Stage  CToolTest CMiniGame3
			m_pStartButton->Set_Click(false);
			CScene*		pScene = CChange_Stage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			//m_SceneType = SCENE_TOOLTEST;

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
			Engine::StopSound(SOUND_BGM);
		
			return 0;
		}
		

		else if (m_pSettingButton->Get_Click())
		{
			m_pSettingButton->Set_Click(false);
			CScene*		pScene = CSetting_Stage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			m_SceneType = SCENE_TOOLTEST;
												// Logo    Setting_Stage
			FAILED_CHECK_RETURN(Engine::Change_Scene(static_cast<CScene*>(this), pScene), E_FAIL);
			Engine::StopSound(SOUND_BGM);
			return 0;
		}
		
		/*if (m_pExitButton->Get_Click())
		{
			DestroyWindow
			return 0;
		}*/
	}	
	return iResult;
}

void CLogo::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CLogo::Render_Scene(void)
{

	if (m_iCount > 20)
		m_bRender = true;


	if (m_bRender)	
		Render_Font(L"Font_Jinji", m_pLoading->Get_String(), &_vec2(350.f, 800.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


	if (m_iCount > 50)
	{
		m_bRender = false;
		m_iCount = 0;
	}
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	CGameObject*		pGameObject = nullptr;

	// backGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	pGameObject = CStart_Button::Create(m_pGraphicDev, -0.6f, 0.55f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartButton", pGameObject), E_FAIL);

	pGameObject = CSettingButton::Create(m_pGraphicDev, -0.6f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SettingButton", pGameObject), E_FAIL);

	pGameObject = CExit_Button::Create(m_pGraphicDev, -0.6f, -0.55f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ExitButton", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}

HRESULT CLogo::Ready_Proto(void)
{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_OrthoTransformCom", COrthoTransform::Create()), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTexCom", CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/Title_Menu1.png", TEX_NORMAL)), E_FAIL);

		// Button											
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ButtonTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/Start_Button.png", TEX_NORMAL)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CreditButtonTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/Credits_Button.png", TEX_NORMAL)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ButtonTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/Exit_Button.png", TEX_NORMAL)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BackButtonTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/Back_Button.png", TEX_NORMAL)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BGM_Dn_BtnTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/BGM_Dn_Btn.png", TEX_NORMAL)), E_FAIL);
		
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BGM_Up_BtnTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/BGM_Up_Btn.png", TEX_NORMAL)), E_FAIL);
		// Font
		FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"BMYEONSUNG", L"Power", 14, 18, FW_HEAVY), E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"DalseoHealingBold", L"Healing", 13, 18, FW_NORMAL), E_FAIL);

		FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"HoengseongHanu", L"BulletUIFont", 20, 25, FW_NORMAL), E_FAIL);
		// LeeSoonSin
		FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"LeeSoonSin", L"LoadingHUD_Font", 18, 25, FW_HEAVY), E_FAIL);
		// Ghanachocolate
		FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Ghanachocolate", L"SoundSettings_Font", 15, 20, FW_HEAVY), E_FAIL);
		// BMJUA_ttf
		FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"BMJUA_ttf", L"PlayerDead_Font", 29, 30, FW_HEAVY), E_FAIL);


		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderRectCom", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFilesPath/ShaderRect.hlsl"))), E_FAIL);


		return S_OK;
}
