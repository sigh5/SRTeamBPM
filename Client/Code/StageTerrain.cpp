#include "stdafx.h"
#include "..\Header\StageTerrain.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "MiniTerrain.h"

#include "MagneticField.h"
#include "MsgUI.h"
#include "MiniPlayer.h"
#include "UI_Timer.h"
CStageTerrain::CStageTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStageTerrain::~CStageTerrain()
{
}

HRESULT CStageTerrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vScale = { 0.3f, 0.3f,0.3f };
	m_pTransCom->Set_Scale(&vScale);

	return S_OK;
}

_int CStageTerrain::Update_Object(const _float & fTimeDelta)
{
	if (!m_bOnce)
	{
		Create_Once();
		m_bOnce = true;
	}

	m_fFrame += 1.f*fTimeDelta;


	if (m_fFrame >= 10.f)
	{
		if (m_pCurrentField != nullptr)
			m_pCurrentField->Set_Active(false);


		CScene* pScene = Get_Scene();
		CLayer* pLayer = pScene->GetLayer(L"Layer_GameLogic");

		_int iRandomNum = 0; 

		_int iindexNum = rand() % 9;
		
		if (m_iPreNum == iindexNum)
		{
			while (m_iPreNum == iindexNum)
			{
				iindexNum = rand() % 9;
			}
		}

		m_iPreNum = iindexNum;

		for (auto iter : pLayer->Get_ControlRoomList())
		{
			if (iindexNum == iRandomNum)
			{
				static_cast<CMagneticField*>(iter)->Set_Active(true);
				break;
			}
			iRandomNum++;
		}
	
		m_fFrame = 0.f;


	}

	CScene*pScene = ::Get_Scene();
	CLayer* pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CMiniPlayer* pPlayer = static_cast<CMiniPlayer*>(pLayer->Get_GameObject(L"Player"));

	pLayer = pScene->GetLayer(L"Layer_UI");
	CUI_Timer* pTimer = static_cast<CUI_Timer*>(pLayer->Get_GameObject(L"Timer"));
	//MSG_BOX("8bit");
	if (!m_bPlayerGetSkillCheck[0] && pPlayer->Get_MonsterKillCount() >= 50)
	{
		Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);
		CMsgUI *pMsgBox = dynamic_cast<CMsgUI*>(pLayer->Get_GameObject(L"MsgUI3"));
		if (pMsgBox != nullptr)
		{

			pMsgBox->Set_Active(true);
			pMsgBox->Set_MsgFrame(WINCX / 2.f, WINCY / 2.f, 600.f, 500.f);
			pMsgBox->Set_FontMsg(L"50킬!!\n대쉬 해제", WINCX / 2.f - 75.f, WINCY / 2.f - 15.f);
		}
		m_bPlayerGetSkillCheck[0] = true;
	}
	else 	if (!m_bPlayerGetSkillCheck[1] && pPlayer->Get_MonsterKillCount() >=100)
	{
		Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);
		CMsgUI *pMsgBox = dynamic_cast<CMsgUI*>(pLayer->Get_GameObject(L"MsgUI3"));
		if (pMsgBox != nullptr)
		{

			pMsgBox->Set_Active(true);
			pMsgBox->Set_MsgFrame(WINCX / 2.f, WINCY / 2.f, 600.f, 500.f);
			pMsgBox->Set_FontMsg(L"100킬!!\n총알 생성시간감소", WINCX / 2.f - 100.f, WINCY / 2.f - 15.f);
		}
		m_bPlayerGetSkillCheck[1] = true;
	}

	else 	if (!m_bPlayerGetSkillCheck[2] && pPlayer->Get_MonsterKillCount() >= 75)
	{
		Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);
		CMsgUI *pMsgBox = dynamic_cast<CMsgUI*>(pLayer->Get_GameObject(L"MsgUI3"));
		if (pMsgBox != nullptr)
		{

			pMsgBox->Set_Active(true);
			pMsgBox->Set_MsgFrame(WINCX / 2.f, WINCY / 2.f, 600.f, 500.f);
			pMsgBox->Set_FontMsg(L"75킬!!\n버기탄 사용", WINCX / 2.f - 75.f, WINCY / 2.f -15.f );
		}
		m_bPlayerGetSkillCheck[2] = true;
	}
	else 	if (!m_bPlayerGetSkillCheck[3] && pTimer->Get_Time() >=60.f)
	{
		Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);
		CMsgUI *pMsgBox = dynamic_cast<CMsgUI*>(pLayer->Get_GameObject(L"MsgUI3"));
		if (pMsgBox != nullptr)
		{

			pMsgBox->Set_Active(true);
			pMsgBox->Set_MsgFrame(WINCX / 2.f, WINCY / 2.f, 600.f, 500.f);
			pMsgBox->Set_FontMsg(L"김포 라이더\n배인희", WINCX / 2.f - 75.f, WINCY / 2.f-10.f );
		}
		m_bPlayerGetSkillCheck[3] = true;
	}

	else 	if (!m_bPlayerGetSkillCheck[4] && pTimer->Get_Time() >= 90.f)
	{
		Engine::PlaySoundW(L"Currect_Mark.wav", SOUND_EFFECT, 1.f);
		CMsgUI *pMsgBox = dynamic_cast<CMsgUI*>(pLayer->Get_GameObject(L"MsgUI3"));
		if (pMsgBox != nullptr)
		{

			pMsgBox->Set_Active(true);
			pMsgBox->Set_MsgFrame(WINCX / 2.f, WINCY / 2.f, 600.f, 500.f);
			pMsgBox->Set_FontMsg(L"파주 맛돌이\n박종욱", WINCX / 2.f - 75.f, WINCY / 2.f-10.f );
		}
		m_bPlayerGetSkillCheck[4] = true;
	}


	Engine::CGameObject::Update_Object(fTimeDelta);
	
	::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CStageTerrain::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CStageTerrain::Render_Obejct(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(::Get_GameObject(L"Layer_Environment", L"StaticCamera"));

	_matrix			WorldMatrix, ViewMatrix, ViewMatrixInv, ProjMatrix;

	m_pTransCom->Get_WorldMatrix(&WorldMatrix);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->Set_Raw_Value("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_vCamPosition", (_vec3*)&ViewMatrixInv.m[3][0], sizeof(_vec3));


	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 2);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pShaderCom->Begin_Shader(0);

	m_pBufferCom->Render_Buffer();

	m_pShaderCom->End_Shader();
}



HRESULT CStageTerrain::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(::Clone_Proto(L"Proto_TerrainTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(::Clone_Proto(L"Proto_TerrainTexture2"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture2", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(::Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(::Clone_Proto(L"Proto_ShaderTerrain"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_ShaderTerrain", pComponent });

	return S_OK;
}

void CStageTerrain::Create_Once()
{
	CScene* pScene = Get_Scene();
	_vec3 vPos;
	_vec3 vScale = { 0.05f,0.05f,0.05f };
	CLayer*  pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CMiniTerrain* pGameObject = CMiniTerrain::Create(m_pGraphicDev);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniTerrain3", pGameObject), );
	static_cast<CMiniTerrain*>(pGameObject)->InitScaleAndPos(&vScale, &(m_pBufferCom->Get_Center() * 0.25f), true);

	pLayer = pScene->GetLayer(L"Layer_GameLogic");
	
	vPos = m_pBufferCom->Get_Center() * 0.1f;
	CMagneticField* pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);

	vPos.x = m_pBufferCom->Get_Center().x * 0.1f;
	vPos.z = m_pBufferCom->Get_Center().z * 0.3f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);

	vPos.x = m_pBufferCom->Get_Center().x * 0.1f;
	vPos.z = m_pBufferCom->Get_Center().z * 0.5f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);

	vPos = m_pBufferCom->Get_Center() * 0.3f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pField->m_iOption = 1;
	pLayer->Add_ControlRoomList(pField);

	vPos.x = m_pBufferCom->Get_Center().x * 0.3f;
	vPos.z = m_pBufferCom->Get_Center().z * 0.1f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);

	vPos.x = m_pBufferCom->Get_Center().x * 0.3f;
	vPos.z = m_pBufferCom->Get_Center().z * 0.5f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);


	vPos = m_pBufferCom->Get_Center() * 0.5f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);
	
	vPos.x = m_pBufferCom->Get_Center().x * 0.5f;
	vPos.z = m_pBufferCom->Get_Center().z * 0.1f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);


	vPos.x = m_pBufferCom->Get_Center().x * 0.5f;
	vPos.z = m_pBufferCom->Get_Center().z * 0.3f;
	pField = CMagneticField::Create(m_pGraphicDev, vPos);
	pLayer->Add_ControlRoomList(pField);



}

CStageTerrain * CStageTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageTerrain *	pInstance = new CStageTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CStageTerrain::Free(void)
{
	CGameObject::Free();
}
