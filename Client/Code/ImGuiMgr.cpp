#include "stdafx.h"
#include "ImGuiMgr.h"
#include "ImGuizmo.h"
#include "imgui.h"

#include "Scene.h"
#include "Export_Function.h"
#include "Terrain.h"

#include "Texture.h"
#include "Anubis.h"


#include "FileIOMgr.h"
#include "WallCube.h"
#include "MonsterToolObject.h"
#include "Obstacle.h"
#include "ControlRoom.h"


IMPLEMENT_SINGLETON(CImGuiMgr)

ImGuiTextBuffer CImGuiMgr::log;

bool CImGuiMgr::Show_Terrain_Window = true;
bool CImGuiMgr::Show_Player_Window = false;
bool CImGuiMgr::Show_Main_Menu_Window = false;
bool CImGuiMgr::Show_Cube_Tool = false;
bool CImGuiMgr::Show_Monster_Tool = false;
bool CImGuiMgr::Show_Object_Tool = false;

_int CImGuiMgr::m_iInterval = 100;
_int CImGuiMgr::m_iWidth = 100;
_int CImGuiMgr::m_iDepth = 1;


static _bool  g_Show_Sphere = false;


ImVec4 CImGuiMgr::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

CImGuiMgr::CImGuiMgr()
{
	m_CurrentUser = USER_HHW;
}

CImGuiMgr::~CImGuiMgr()
{
	for (auto iter : NameList)
	{
		Safe_Delete_Array(iter);
	}
	Free();
}

HRESULT CImGuiMgr::Ready_MapTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene)
{
	CLayer *pLayer = Engine::CLayer::Create();
	pScene->Add_Layer(pLayer, L"MapCubeLayer");

	CLayer* TelePortpLayer = Engine::CLayer::Create();
	pScene->Add_Layer(TelePortpLayer, L"MapTeleportLayer");

	CLayer* TerrainLayer = Engine::CLayer::Create();
	pScene->Add_Layer(TerrainLayer, L"TerrainLayer");

	CLayer* ControlLayer = Engine::CLayer::Create();
	pScene->Add_Layer(ControlLayer, L"ControlLayer");

	return S_OK;
}

HRESULT CImGuiMgr::Ready_PlayerTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture2", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Player/Select/Player%d.jpg", TEX_NORMAL, 6)), E_FAIL);

	return S_OK;
}

HRESULT CImGuiMgr::Ready_MonsterTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene)
{
	CLayer *pLayer = Engine::CLayer::Create();
	pScene->Add_Layer(pLayer, L"TestLayer3");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture1", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Monster/Example/Example_%d.png", TEX_NORMAL, 3)), E_FAIL);
	return S_OK;
}

HRESULT CImGuiMgr::Ready_ObjectTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene)
{
	CLayer *pLayer = Engine::CLayer::Create();
	pScene->Add_Layer(pLayer, L"ObjectLayer");

	return S_OK;
}

void CImGuiMgr::TransformEdit(CCamera* pCamera, CTransform* pTransform, _bool& Window)
{
	if (Window == false)
		return;

	if (g_Show_Sphere)
	{
		ImGui::Begin("Transform");
		ImGuizmo::BeginFrame();
		static float snap[3] = { 1.f, 1.f, 1.f };
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		if (pTransform == nullptr)
		{
			ImGui::Text("Object Delete or nullptr");
			ImGui::End();
			return;
		}

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		_matrix matWorld = pTransform->m_matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matWorld);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		static bool useSnap(false);
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("##something", &useSnap);
		ImGui::SameLine();
		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}

		if (ImGui::Button("Close"))
		{
			Window = false;
		}


		_matrix matId;
		D3DXMatrixIdentity(&matId);

		ImGuiIO& io = ImGui::GetIO();
		RECT rt;
		GetClientRect(g_hWnd, &rt);
		POINT lt{ rt.left, rt.top };
		ClientToScreen(g_hWnd, &lt);
		ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, (_float)io.DisplaySize.x, (_float)io.DisplaySize.y);

		// ImGuizmo::DrawGrid(m_pCam->GetView(), m_pCam->GetPrj(), matId, 100.f);

		ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

		pTransform->m_matWorld = matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		matrixRotation[0] = D3DXToRadian(matrixRotation[0]);
		matrixRotation[1] = D3DXToRadian(matrixRotation[1]);
		matrixRotation[2] = D3DXToRadian(matrixRotation[2]);
		memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
		memcpy(&pTransform->m_vAngle, matrixRotation, sizeof(matrixRotation));
		memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));




		ImGui::End();
	}


	else if (true == Show_Cube_Tool || Show_Object_Tool)
	{
		ImGui::Begin("Transform");
		ImGuizmo::BeginFrame();
		static float snap[3] = { 1.f, 1.f, 1.f };
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		if (pTransform == nullptr)
		{
			ImGui::Text("Object Delete or nullptr");
			ImGui::End();
			return;
		}

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		_matrix matWorld = pTransform->m_matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matWorld);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		static bool useSnap(false);
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("##something", &useSnap);
		ImGui::SameLine();
		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}

		if (ImGui::Button("Close"))
		{
			Window = false;
		}


		_matrix matId;
		D3DXMatrixIdentity(&matId);

		ImGuiIO& io = ImGui::GetIO();
		RECT rt;
		GetClientRect(g_hWnd, &rt);
		POINT lt{ rt.left, rt.top };
		ClientToScreen(g_hWnd, &lt);
		ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, (_float)io.DisplaySize.x, (_float)io.DisplaySize.y);

		// ImGuizmo::DrawGrid(m_pCam->GetView(), m_pCam->GetPrj(), matId, 100.f);

		ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

		pTransform->m_matWorld = matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		matrixRotation[0] = D3DXToRadian(matrixRotation[0]);
		matrixRotation[1] = D3DXToRadian(matrixRotation[1]);
		matrixRotation[2] = D3DXToRadian(matrixRotation[2]);
		memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
		memcpy(&pTransform->m_vAngle, matrixRotation, sizeof(matrixRotation));
		memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));




		ImGui::End();
	}
	else if (true == Show_Monster_Tool)
	{
		ImGui::Begin("Transform");
		ImGuizmo::BeginFrame();
		static float snap[3] = { 1.f, 1.f, 1.f };
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		//if (ImGui::IsKeyPressed(69))
		//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		/*if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;*/
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		if (pTransform == nullptr)
		{
			ImGui::Text("Object Delete or nullptr");
			ImGui::End();
			return;
		}


		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		_matrix matWorld = pTransform->m_matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		matrixScale[2] = 1.0f;    //부동소수점 문제로 인해 일단 잠굼
		ImGui::InputFloat3("Tr", matrixTranslation);
		//ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matWorld);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		static bool useSnap(false);
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("##something", &useSnap);
		ImGui::SameLine();
		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
			//case ImGuizmo::ROTATE:
			//	ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}

		if (ImGui::Button("Close"))
		{
			Window = false;
		}


		_matrix matId;
		D3DXMatrixIdentity(&matId);

		ImGuiIO& io = ImGui::GetIO();
		RECT rt;
		GetClientRect(g_hWnd, &rt);
		POINT lt{ rt.left, rt.top };
		ClientToScreen(g_hWnd, &lt);
		ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, (_float)io.DisplaySize.x, (_float)io.DisplaySize.y);

		// ImGuizmo::DrawGrid(m_pCam->GetView(), m_pCam->GetPrj(), matId, 100.f);

		ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

		pTransform->m_matWorld = matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
		memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));


		ImGui::End();
	}
	else if (true == Show_Terrain_Window)
	{
		ImGui::Begin("Transform");
		ImGuizmo::BeginFrame();
		static float snap[3] = { 1.f, 1.f, 1.f };
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		if (pTransform == nullptr)
		{
			ImGui::Text("Object Delete or nullptr");
			ImGui::End();
			return;
		}

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		_matrix matWorld = pTransform->m_matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matWorld);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		static bool useSnap(false);
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("##something", &useSnap);
		ImGui::SameLine();
		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}

		if (ImGui::Button("Close"))
		{
			Window = false;
		}


		_matrix matId;
		D3DXMatrixIdentity(&matId);

		ImGuiIO& io = ImGui::GetIO();
		RECT rt;
		GetClientRect(g_hWnd, &rt);
		POINT lt{ rt.left, rt.top };
		ClientToScreen(g_hWnd, &lt);
		ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, (_float)io.DisplaySize.x, (_float)io.DisplaySize.y);

		// ImGuizmo::DrawGrid(m_pCam->GetView(), m_pCam->GetPrj(), matId, 100.f);

		ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

		pTransform->m_matWorld = matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		matrixRotation[0] = D3DXToRadian(matrixRotation[0]);
		matrixRotation[1] = D3DXToRadian(matrixRotation[1]);
		matrixRotation[2] = D3DXToRadian(matrixRotation[2]);
		memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
		memcpy(&pTransform->m_vAngle, matrixRotation, sizeof(matrixRotation));
		memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));




		ImGui::End();
	}



}

void CImGuiMgr::LoggerWindow()
{
	ImGui::Begin("Example: Long text display");
	static int test_type = 0;
	static int lines = 0;
	ImGui::Text("Printing unusually long amount of text.");
	ImGui::Combo("Test type", &test_type,
		"Single call to TextUnformatted()\0"
		"Multiple calls to Text(), clipped\0"
		"Multiple calls to Text(), not clipped (slow)\0");
	ImGui::Text("Buffer contents: %d lines, %d bytes", lines, log.size());
	if (ImGui::Button("Clear")) { log.clear(); lines = 0; }
	ImGui::SameLine();
	if (ImGui::Button("Add 1000 lines"))
	{
		for (int i = 0; i < 1000; i++)
			log.appendf("%i The quick brown fox jumps over the lazy dog\n", lines + i);
		lines += 1000;
	}
	ImGui::BeginChild("Log");
	switch (test_type)
	{
	case 0:
		ImGui::TextUnformatted(log.begin(), log.end());
		break;
	case 1:
	{
		// Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGuiListClipper clipper;
		clipper.Begin(lines);
		while (clipper.Step())
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
		ImGui::PopStyleVar();
		break;
	}
	case 2:
		// Multiple calls to Text(), not clipped (slow)
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		for (int i = 0; i < lines; i++)
			ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
		ImGui::PopStyleVar();
		break;
	}
	ImGui::EndChild();
	ImGui::End();
}

void CImGuiMgr::MultipleTransformEdit(CCamera * pCamera, std::list<pair<const wstring, CGameObject*>> Multiplelist, _bool & Window)
{
	if (Multiplelist.empty())
	{
		ImGui::Text("Object Delete or nullptr");
		ImGui::End();
		return;
	}

	ImGui::Begin("Transform");

	ImGuizmo::BeginFrame();

	static float snap[3] = { 1.f, 1.f, 1.f };
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::SCALE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	/*if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;*/
	if (ImGui::IsKeyPressed(82)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	//if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
	//	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	//if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
	//	mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	static bool useSnap(false);
	if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;
	ImGui::Checkbox("##something", &useSnap);
	ImGui::SameLine();
	switch (mCurrentGizmoOperation)
	{
		/*case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;*/
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}

	if (ImGui::Button("Close"))
	{
		Window = false;
	}


	CTransform* pTransform = dynamic_cast<CTransform*>(Multiplelist.begin()->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	if (pTransform == nullptr)
		return;

	_matrix matWorld = pTransform->m_matWorld;

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);

	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matWorld);


	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}



	for (auto iter = Multiplelist.begin(); iter != Multiplelist.end(); ++iter)
	{
		_matrix matId;
		D3DXMatrixIdentity(&matId);

		ImGuiIO& io = ImGui::GetIO();
		RECT rt;
		GetClientRect(g_hWnd, &rt);
		POINT lt{ rt.left, rt.top };
		ClientToScreen(g_hWnd, &lt);
		ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, (_float)io.DisplaySize.x, (_float)io.DisplaySize.y);

		ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

		pTransform = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		pTransform->m_matWorld = matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));
	}

	ImGui::End();


}

void CImGuiMgr::WindowLayOut()
{
	static float f = 0.0f;
	static int counter = 0;
	//RadioButton
	static int RadioButton;
	int HHW = 0;
	int BIH = 1;
	int PJW = 2;
	//~RadioButton
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Terrain_Tool", &Show_Terrain_Window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("PlayerTool", &Show_Player_Window);
	ImGui::Checkbox("Main_MenuTool", &Show_Main_Menu_Window);
	ImGui::Checkbox("Cube_Tool", &Show_Cube_Tool);
	ImGui::Checkbox("Monster_Tool", &Show_Monster_Tool);
	ImGui::Checkbox("Object_Tool", &Show_Object_Tool);


	if (ImGui::Button("Clear"))
	{
		Show_Player_Window = false;
		Show_Terrain_Window = false;
		Show_Main_Menu_Window = false;
		Show_Cube_Tool = false;
		Show_Monster_Tool = false;
		Show_Object_Tool = false;
	}



	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::BeginChild("USER");
	//유저 구분 라디오 버튼
	if (ImGui::RadioButton("HHW", &HHW, m_CurrentUser))
	{
		m_CurrentUser = USER_HHW;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("BIH", &BIH, m_CurrentUser))
	{
		m_CurrentUser = USER_BIH;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("PJW", &PJW, m_CurrentUser))
	{
		m_CurrentUser = USER_PJW;
	}
	ImGui::EndChild();
	ImGui::End();
}


void CImGuiMgr::CreateObject(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera* pCam, wstring pObjectName)
{
	if (!Show_Cube_Tool)
		return;

	ImGui::Begin("Cube Settings");

	ImGui::Text("this is Transform_ButtonMenu");
	if (ImGui::Button("Save"))
		Save_CubeMap(pScene);
	ImGui::SameLine();

	if (ImGui::Button("Load"))
		Load_CubeMap(pGrahicDev, pScene);
	ImGui::SameLine();

	if (ImGui::Button("Delete"))
	{
		CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
		HRESULT Temp = MyLayer->Delete_GameObject(m_CurrentSelectGameObjectObjKey.c_str());

		if (Temp == S_OK)
		{
			m_pWallCube = nullptr;
			m_pSelectedObject = nullptr;
			m_pSelectedTransform = nullptr;
		}

	}


	if (ImGui::Button("Create Sphere"))
	{
		g_Show_Sphere = !g_Show_Sphere;
	}


	if (g_Show_Sphere)
	{
		Create_Sphere(pGrahicDev, pScene, pCam, pObjectName);
	}







	if (ImGui::CollapsingHeader("Cube Create & Chose Button", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Create"))
		{
			m_bCubeCreateCheck = true;
			m_bCubeSelcetCheck = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("ChoseCube"))
		{
			m_bCubeSelcetCheck = true;
			m_bCubeCreateCheck = false;
		}
	}

	CTransform * pTranscom = nullptr;

	if (m_bCubeCreateCheck && !g_Show_Sphere)
	{
		Set_CubeDir();

		CLayer* pLayer = pScene->GetLayer(L"MapCubeLayer");
		Set_All_CubeHeight(pLayer);

		ImGui::Text("if double click Create Cube");
		if (ImGui::IsMouseDoubleClicked(0))
		{
			CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
			CWallCube *pWallCube = nullptr;
			CGameObject *pGameObject = nullptr;
			_bool	isUpcube = false;

			if (-1 == CheckCubeinPicking<CWallCube>(MyLayer, &pWallCube, &isUpcube))
				return;

			ImVec2 temp = ImGui::GetMousePos();
			_vec2 vec2MousePos = { temp.x,temp.y };
			ObjectCreate<CWallCube>(pGrahicDev, MyLayer, &pGameObject, pObjectName);
			static_cast<CWallCube*>(pGameObject)->InitSetting(&vec2MousePos, L"TerrainLayer", &m_CurrentTerrainObjectName);

			static_cast<CWallCube*>(pGameObject)->Set_DrawTexIndex(m_iMapCubeIndex);

			if (pGameObject != nullptr && pWallCube != nullptr)
				Set_Create_Cube_Pos(&pGameObject, &pWallCube, &isUpcube);
		}
	}

	if (m_bCubeSelcetCheck)
	{
		if ((Get_DIKeyState(DIK_N) & 0x80))
		{
			for (auto iter = m_SelecteObjectlist.begin(); iter != m_SelecteObjectlist.end(); ++iter)
			{
				iter->second->Set_WireFrame(false);
			}
			m_SelecteObjectlist.clear();
			m_bMultiple_Select = false;
		}

		if (ImGui::IsMouseClicked(0) && !(Get_DIKeyState(DIK_LCONTROL) & 0x80))
		{

			CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
			(m_pWallCube) = dynamic_cast<CWallCube*>(SelectObject<CWallCube>(MyLayer, &m_CurrentObjectName));

			

		}

		if (Get_DIKeyState(DIK_LCONTROL) & 0x80 && ImGui::IsMouseClicked(0))
		{
			CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
			CGameObject * pMultiPleObject = nullptr;
			pMultiPleObject = (SelectObject<CWallCube>(MyLayer, &m_CurrentObjectName));
			m_SelecteObjectlist.push_back({ m_CurrentObjectName, pMultiPleObject });
			m_bMultiple_Select = true;
		}

	}

	if (m_pWallCube && !m_bMultiple_Select)
	{
		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
			Set_Cube_WireFrame(MyLayer);
		}

		ImGui::NewLine();
		ImGui::Text("Choose Option");
		static _int iCubeOption = 0;			// 한번 함수돌고 다시 0되면 안되므로  static으로 씀
		ImGui::InputInt("Snap", &iCubeOption);

		if (ImGui::Button("CubeOption Change"))
		{
			m_pWallCube->Set_Option((CUBE_TYPE)iCubeOption);
		}

		ImGui::Text("Current choose Cube Option : ");
		ImGui::SameLine();
		char szOption[20];
		sprintf_s(szOption, "%d", m_pWallCube->Get_Option());

		ImGui::Text(szOption);
		EditObjectTexture<CWallCube>(L"Proto_MapCubeTexture");
		TransformEdit(pCam, m_pSelectedTransform, Show_Cube_Tool);
	}

	else if (m_bMultiple_Select && !m_SelecteObjectlist.empty())
	{
		ImGui::NewLine();
		ImGui::Text("Choose Option");
		static _int iCubeOption = 0;			// 한번 함수돌고 다시 0되면 안되므로  static으로 씀
		ImGui::InputInt("Snap", &iCubeOption);

		ImGui::Text("Current choose Cube Option : ");
		ImGui::SameLine();
		char szOption[20];

		//sprintf_s(szOption, "%d", dynamic_cast<CWallCube*>(m_SelecteObjectlist.begin()->second)->Get_Option());

		ImGui::Text(szOption);

		for (auto iter = m_SelecteObjectlist.begin(); iter != m_SelecteObjectlist.end(); ++iter)
		{
			iter->second->Set_WireFrame(true);
		}



		MultiPleEditObjectTexture<CWallCube>(L"Proto_MapCubeTexture");

		// 텍스쳐 다중으로 바꾸기 가능 
		for (auto iter = m_SelecteObjectlist.begin(); iter != m_SelecteObjectlist.end(); ++iter)
		{
			CTransform* pTransfromcom = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		}

		MultipleTransformEdit(pCam, m_SelecteObjectlist, Show_Cube_Tool);

	}
	else
	{



		ImGui::End();
		return;
	}







	//if (pTranscom != nullptr)
	//	m_pSelectedTransform = pTranscom;

	ImGui::End();
}

void CImGuiMgr::TerrainTool(LPDIRECT3DDEVICE9 pGrahicDev, CCamera* pCam, CScene* pScene)
{
	if (!Show_Terrain_Window)
		return;

	ImGui::Begin("Terrain Settings");


	if (ImGui::Button("Save"))
		CFileIOMgr::GetInstance()->Save_FileData(pScene, L"TerrainLayer", L"../../Data/", L"Stage1Room.dat", OBJ_ROOM);
	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
			pScene,
			L"TerrainLayer",
			L"../../Data/",
			L"Stage1Room.dat",
			L"StageRoom",
			OBJ_ROOM);
	}

	if (ImGui::CollapsingHeader("Tile Count", ImGuiTreeNodeFlags_DefaultOpen))
	{
		CLayer* pLayer = pScene->GetLayer(L"TerrainLayer");
		ImGui::SliderInt("Width", &m_iWidth, 0, 200);
		ImGui::SliderInt("Depth", &m_iDepth, 0, 200);
		ImGui::SliderInt("Interval", &m_iInterval, 0, 10);
		ImGui::NewLine();

		if (ImGui::Button("Create"))
		{
			CGameObject* pGameObject = nullptr;
			ObjectCreate<CTerrain>(pGrahicDev, pLayer, &pGameObject, L"TerrainObj");
		}
	}

	if (ImGui::Button("Delete"))
	{
		CLayer* MyLayer = pScene->GetLayer(L"TerrainLayer");
		MyLayer->Delete_GameObject(m_CurrentTerrainObjectName.c_str());
		m_pSelectedObject = nullptr;
	}


	if (ImGui::IsMouseClicked(1))
	{
		CLayer* pLayer = pScene->GetLayer(L"TerrainLayer");
		CGameObject* temp = SelectObject<CTerrain>(pLayer, &m_CurrentTerrainObjectName);
	}

	CTerrain *pTerrain = dynamic_cast<CTerrain*>(m_pSelectedObject);

	if (pTerrain != nullptr)
	{
		EditObjectTexture<CTerrain>(L"Proto_TerrainTexture2");
		TransformEdit(pCam, m_pSelectedTransform, Show_Terrain_Window);
	}









	ImGui::End();
}

void CImGuiMgr::MonsterTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene * pScene, CCamera *pCam)
{
	if (!Show_Monster_Tool)
		return;

	ImGui::Begin("Monster Settings");

	ImGui::Text("this is Transform_ButtonMenu");
	if (ImGui::Button("Save"))
	{
		switch (m_CurrentUser)
		{
		case USER_HHW:
			CFileIOMgr::GetInstance()->Save_FileData(pScene,
				L"TestLayer3",
				L"../../Data/",
				L"Monster1.dat",
				OBJ_MONSTER);
			break;

		case USER_BIH:
			CFileIOMgr::GetInstance()->Save_FileData(pScene,
				L"TestLayer3",
				L"../../Data/",
				L"Monster2.dat",
				OBJ_MONSTER);
			break;

		case USER_PJW:
			CFileIOMgr::GetInstance()->Save_FileData(pScene,
				L"TestLayer3",
				L"../../Data/",
				L"Monster3.dat",
				OBJ_MONSTER);
			break;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		switch (m_CurrentUser)
		{
		case USER_HHW:
			CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
				pScene,
				L"TestLayer3",
				L"../../Data/",
				L"Monster1.dat",
				L"TestMonster",
				OBJ_MONSTER);
			break;
		case USER_BIH:
			CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
				pScene,
				L"TestLayer3",
				L"../../Data/",
				L"Monster2.dat",
				L"TestMonster",
				OBJ_MONSTER);
			break;
		case USER_PJW:
			CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
				pScene,
				L"TestLayer3",
				L"../../Data/",
				L"Monster3.dat",
				L"TestMonster",
				OBJ_MONSTER);
			break;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		CLayer* MyLayer = pScene->GetLayer(L"TestLayer3");
		MyLayer->Delete_GameObject(m_CurrentSelectGameObjectObjKey.c_str());
	}

	if (ImGui::CollapsingHeader("Monster Create & Chose Button", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("MonsterDeploy"))
		{
			m_bMonsterCreateCheck = true;
			m_bMonsterSelectCheck = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("SelectMonster"))
		{
			m_bMonsterSelectCheck = true;
			m_bMonsterCreateCheck = false;
		}
	}
	CLayer* pLayer = pScene->GetLayer(L"TestLayer3");
	CTransform * pTranscom = nullptr;
	if (m_bMonsterCreateCheck)
	{
		ImGui::Text("if double click Create Cube");
		if (ImGui::IsMouseDoubleClicked(0))
		{
			CMonsterToolObject* pMonsterObj = nullptr;
			CGameObject*		pGameObject = nullptr;
			_bool isUpcube = false;

			ImVec2 temp = ImGui::GetMousePos();
			_vec2 vec2MousePos = { temp.x,temp.y };
			ObjectCreate<CMonsterToolObject>(pGrahicDev, pLayer, &pGameObject, L"TestLayer3");
			static_cast<CMonsterToolObject*>(pGameObject)->InitSetting(&vec2MousePos, L"TerrainLayer", &m_CurrentTerrainObjectName);

		}
	}

	if (m_bMonsterSelectCheck)
	{
		if (ImGui::IsMouseClicked(0))
		{
			CLayer* pMonsterlayer = pScene->GetLayer(L"TestLayer3");

			map<const _tchar*, CGameObject*> test = pMonsterlayer->Get_GameObjectMap();

			for (auto iter = test.begin(); iter != test.end(); ++iter)
			{
				if (dynamic_cast<CMonsterToolObject*>(iter->second)->Set_SelectGizmo(g_hWnd, static_cast<CMonsterToolObject*>(iter->second)->Get_Calculator(), static_cast<CMonsterToolObject*>(iter->second)->Get_Buffer()))
				{
					pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
					m_CurrentSelectGameObjectObjKey = iter->first;
				}
			}
		}
	}
	CGameObject* pGameObject = dynamic_cast<CMonsterToolObject*>(Engine::Get_GameObject(L"TestLayer3", m_CurrentSelectGameObjectObjKey.c_str()));

	ImGui::NewLine();

	//if (ImGui::CollapsingHeader("Monster Texture", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_TerrainTexture2", ID_STATIC));

	//	vector<IDirect3DBaseTexture9*> vecTexture = pTextureCom->Get_Texture();

	//	for (_uint i = 0; i < 18; ++i)
	//	{
	//		if (ImGui::ImageButton((void*)vecTexture[i], ImVec2(32.f, 32.f)))
	//		{
	//			//pTextureCom->Set_Texture(i);
	//			pGameObject->m_iTerrainIdx = i;
	//		}
	//		if (i == 0 || (i + 1) % 6)
	//			ImGui::SameLine();
	//	}
	//}
	TransformEdit(pCam, m_pSelectedTransform, Show_Monster_Tool);

	if (pTranscom != nullptr)
		m_pSelectedTransform = pTranscom;


	if (pGameObject != nullptr)
	{
		//CharacterInfo* monInfo = nullptr;
		//monInfo = &(static_cast<CMonsterBase*>(pGameObject)->Get_InfoRef());
		ImGui::Begin("Monster Stat");

		ImGui::Text("Monster Stat Setting Window");
		//ImGui::InputInt("Hp", &monInfo->_iHp);
		//ImGui::InputInt("AttackPower", &monInfo->_iAttackPower);
		ImGui::InputInt("MonsterIndex", &(static_cast<CMonsterToolObject*>(pGameObject)->Get_MonsterType()));


		ImGui::End();
	}
	ImGui::End();
}

// Player Tool
void CImGuiMgr::Player_Tool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eId)
{
	if (!Show_Player_Window)
		return;
//
//	ImGui::Begin("Player Setting Manager");
//
//	/*CTestPlayer* pGameObject = dynamic_cast<CTestPlayer*>(Engine::Get_GameObject(L"TestLayer", L"TestPlayer"));
//
//	if (pGameObject)
//	{
//		if (ImGui::CollapsingHeader("Player Texture", ImGuiTreeNodeFlags_DefaultOpen))
//		{
//			CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_PlayerTexture2", ID_STATIC));
//
//			vector<IDirect3DBaseTexture9*> vecPlayerTexture = pTextureCom->Get_Texture();
//
//			for (_uint i = 0; i < 5; ++i)
//			{
//				if (ImGui::ImageButton((void*)vecPlayerTexture[i], ImVec2(40.f, 40.f)))
//				{
//					pGameObject->Set_DrawTexIndex(i);
//				}
//				if (i == 0 || (i + 1) % 3)
//					ImGui::SameLine();
//			}
//		}
//
//		ImGui::Text("this is Transform_ButtonMenu");
//		if (ImGui::Button("Save"))
//		{
//			CFileIOMgr::GetInstance()->Save_FileData(pScene, pLayerTag, L"../../Data/", L"Player.dat", OBJ_PLAYER);
//
//		}
//		ImGui::SameLine();
//		if (ImGui::Button("Load"))
//		{
//
//			CFileIOMgr::GetInstance()->Load_FileData(pGraphicDev,
//				pScene,
//				const_cast<_tchar*>(pLayerTag),
//				L"../../Data/",
//				L"Player.dat",
//				L"TestPlayer",
//				OBJ_PLAYER);
//*/
//
//		
//	}
//	ImGui::End();

}

void CImGuiMgr::Object_Tool(LPDIRECT3DDEVICE9 pGrahicDev, CScene * pScene, CCamera * pCam)
{
	if (false == Show_Object_Tool)
		return;

	ImGui::Begin("Object Tool");

	ImGui::Text("this is Transform_ButtonMenu");
	if (ImGui::Button("Save"))
		CFileIOMgr::GetInstance()->Save_FileData(pScene, L"ObjectLayer",
			L"../../Data/", L"Stage1Obstacle.dat", OBJ_OBSTRACLE);
	ImGui::SameLine();

	if (ImGui::Button("Load"))
		CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev, pScene, 
			L"ObjectLayer", L"../../Data/", L"Stage1Obstacle.dat", L"Obstacle", OBJ_OBSTRACLE);



	if (ImGui::Button("Delete"))
	{
		CLayer* MyLayer = pScene->GetLayer(L"ObjectLayer");
		MyLayer->Delete_GameObject(m_CurrentObstaclName.c_str());
		m_pSelectedObject = nullptr;
	}

	static _int		iWidth = 100;
	static _int		iHeight = 100;

	if (ImGui::CollapsingHeader("Tile Count", ImGuiTreeNodeFlags_DefaultOpen))
	{
		CLayer* pLayer = pScene->GetLayer(L"ObjectLayer");

		ImGui::SliderInt("Width", &iWidth, 0, 1000);
		ImGui::SliderInt("Depth", &iHeight, 0, 1000);

		ImGui::NewLine();


		ImGui::Text("if double click Create Obj");
		if (ImGui::IsMouseDoubleClicked(0))
		{
			CObstacle *pObstacleObj = nullptr;
			CGameObject *pGameObject = nullptr;
			_bool	isUpcube = false;

			if (-1 == CheckCubeinPicking<CObstacle>(pLayer, &pObstacleObj, &isUpcube))
				return;

			ImVec2 temp = ImGui::GetMousePos();
			_vec2 vec2MousePos = { temp.x,temp.y };
			ObjectCreate<CObstacle>(pGrahicDev, pLayer, &pGameObject, L"Obstacle");
			static_cast<CObstacle*>(pGameObject)->InitSetting(&vec2MousePos, L"TerrainLayer", &m_CurrentTerrainObjectName);
		}
	}

	if (ImGui::IsMouseClicked(0))
	{
		CLayer* pLayer = pScene->GetLayer(L"ObjectLayer");
		CGameObject* temp = SelectObject<CObstacle>(pLayer, &m_CurrentObstaclName);
	}

	CObstacle *pTerrain = dynamic_cast<CObstacle*>(m_pSelectedObject);

	if (pTerrain != nullptr)
	{
		static _int iOption = 0;
		ImGui::InputInt("Option", &iOption);
		ImGui::SameLine();
		dynamic_cast<CObstacle*>(pTerrain)->Set_Option(iOption);

		CLayer* pLayer = pScene->GetLayer(L"ObjectLayer");

		for (auto &iter : pLayer->Get_GameObjectMap())
		{
			iter.second->Set_WireFrame(false);
		}

		pTerrain->Set_WireFrame(true);
		EditObjectTexture<CObstacle>(L"Proto_fetrues_Texture");
		TransformEdit(pCam,m_pSelectedTransform, Show_Object_Tool);
	}

	ImGui::End();
}

template<typename T>
_int CImGuiMgr::CheckCubeinPicking(CLayer* pLayer, T** pGameObject, _bool* bUp)
{
	_int iCount = 0;
	map<const _tchar*, CGameObject*> CubeMap = pLayer->Get_GameObjectMap();
	for (auto iter = CubeMap.begin(); iter != CubeMap.end(); ++iter)
	{
		if (dynamic_cast<T*>(iter->second)->Set_SelectGizmo())
		{
			*pGameObject = dynamic_cast<T*>(iter->second);
			iCount++;
			*bUp = true;
			return 3;
		}
	}

	if (iCount > 1)
	{
		ImGui::End();
		return -1;
	}
	//주석지우셈
	return 0;
}

// Cube
void CImGuiMgr::Set_CubeDir()
{
	if (ImGui::Button("UP"))
	{
		cubePlane = CREATECUBE_UP;
	}
	ImGui::SameLine();
	if (ImGui::Button("Down"))
	{
		cubePlane = CREATECUBE_DOWN;
	}
	ImGui::SameLine();
	if (ImGui::Button("Left"))
	{
		cubePlane = CREATECUBE_LEFT;
	}
	ImGui::SameLine();
	if (ImGui::Button("Right"))
	{
		cubePlane = CREATECUBE_RIGHT;
	}

}

void CImGuiMgr::Set_All_CubeHeight(CLayer* pLayer)
{
	static _float fHeight = 0.f;			// 한번 함수돌고 다시 0되면 안되므로  static으로 씀
	ImGui::InputFloat("Snap", &fHeight);
	if (ImGui::Button("Height All Cube Set"))
	{
		map<const _tchar*, CGameObject*> CubeMap = pLayer->Get_GameObjectMap();
		CGameObject *pWallCube = nullptr;

		for (auto iter = CubeMap.begin(); iter != CubeMap.end(); ++iter)
		{
			CTransform* pCubeTrnasform = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
			pCubeTrnasform->Set_Y(fHeight);
		}
	}

}

void CImGuiMgr::Set_Create_Cube_Pos(CGameObject ** pGameObject, CWallCube ** pWallCube, _bool* isUpcube)
{
	if (isUpcube)
	{
		CTransform* pCubeTrnasform = dynamic_cast<CTransform*>((*pGameObject)->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		CTransform* pPreCubeTransform = dynamic_cast<CTransform*>((*pWallCube)->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		_vec3 vPrePos;
		pPreCubeTransform->Get_Info(INFO_POS, &vPrePos);

		switch (cubePlane)
		{
		case Engine::CREATECUBE_LEFT:
			pCubeTrnasform->Set_Pos(vPrePos.x - 1.f, vPrePos.y, vPrePos.z);
			break;
		case Engine::CREATECUBE_RIGHT:
			pCubeTrnasform->Set_Pos(vPrePos.x + 1.f, vPrePos.y, vPrePos.z);
			break;
		case Engine::CREATECUBE_UP:
			pCubeTrnasform->Set_Pos(vPrePos.x, vPrePos.y + 0.5f, vPrePos.z);
			break;
		case Engine::CREATECUBE_DOWN:
			pCubeTrnasform->Set_Pos(vPrePos.x, vPrePos.y - 0.5f, vPrePos.z);
			break;
		case Engine::CREATECUBE_END:
			break;
		default:
			break;
		}
	}
}

void CImGuiMgr::Set_Cube_WireFrame(CLayer * pLayer)
{
	static _bool	bWireFrame = false;
	if (ImGui::Checkbox("WireFrame", &bWireFrame))
	{

		map<const _tchar*, CGameObject*> CubeMap = pLayer->Get_GameObjectMap();

		for (auto iter = CubeMap.begin(); iter != CubeMap.end(); ++iter)
		{
			static_cast<CWallCube*>(iter->second)->Set_WireFrame(bWireFrame);
		}
	}
}

void CImGuiMgr::Load_CubeMap(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene)
{
	switch (m_CurrentUser)
	{
	case USER_HHW:
		/*CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
			pScene,
			L"MapCubeLayer",
			L"../../Data/",
			L"PjwCube.dat",
			L"PjwCube",
			OBJ_CUBE);*/


		CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
			pScene,
			L"MapCubeLayer",
			L"../../Data/",
			L"Stage1Map.dat",
			L"TestCube",
			OBJ_CUBE);

		m_iIndex = CFileIOMgr::GetInstance()->Get_Index();
		++m_iIndex;
		break;

	case USER_BIH:
		CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
			pScene,
			L"MapCubeLayer",
			L"../../Data/",
			L"Stage2Map.dat",
			L"TestCube",
			OBJ_CUBE);
		break;

	case USER_PJW:
		CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
			pScene,
			L"MapCubeLayer",
			L"../../Data/",
			L"Stage3Map.dat",
			L"TestCube",
			OBJ_CUBE);
		break;
	}
}

void CImGuiMgr::Create_Sphere(LPDIRECT3DDEVICE9 pGrahicDev, CScene * pScene, CCamera * pCam, wstring pObjectName)
{



	//if (ImGui::IsMouseDoubleClicked(0))
	//{
	//	CLayer* MyLayer = pScene->GetLayer(L"ControlLayer");

	//	
	//	ImVec2 temp = ImGui::GetMousePos();
	///*	_vec2 vec2MousePos = { temp.x,temp.y };
	//	ObjectCreate<CWallCube>(pGrahicDev, MyLayer, &pGameObject, pObjectName);
	//	static_cast<CWallCube*>(pGameObject)->InitSetting(&vec2MousePos, L"TerrainLayer", &m_CurrentTerrainObjectName);

	//	static_cast<CWallCube*>(pGameObject)->Set_DrawTexIndex(m_iMapCubeIndex);*/

	//	/*	if (pGameObject != nullptr && pWallCube != nullptr)
	//			Set_Create_Cube_Pos(&pGameObject, &pWallCube, &isUpcube);*/



	//}


	//if (pControlSphere != nullptr)
	//{
	//	CTransform* pTransform = dynamic_cast<CTransform*>(pControlSphere->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	//	TransformEdit(pCam, pTransform, g_Show_Sphere);
	//}



}



void CImGuiMgr::Save_CubeMap(CScene *pScene)
{
	switch (m_CurrentUser)
	{
	case USER_HHW:
		CFileIOMgr::GetInstance()->Save_FileData(pScene, L"MapCubeLayer",
			L"../../Data/",
			L"Stage1Map.dat", OBJ_CUBE);
		break;

	case USER_BIH:
		CFileIOMgr::GetInstance()->Save_FileData(pScene, L"MapCubeLayer", L"../../Data/", L"Stage2Map.dat", OBJ_CUBE);
		break;

	case USER_PJW:
		CFileIOMgr::GetInstance()->Save_FileData(pScene, L"MapCubeLayer", L"../../Data/", L"Stage3Map.dat", OBJ_CUBE);
		break;

	}
}

void CImGuiMgr::Free()
{
	for (auto iter : PlayerList)
	{
		Safe_Delete_Array(iter);
	}
}