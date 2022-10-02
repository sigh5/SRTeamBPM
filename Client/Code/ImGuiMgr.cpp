#include "stdafx.h"
#include "ImGuiMgr.h"
#include "ImGuizmo.h"
#include "imgui.h"

#include "Export_Function.h"
#include "Terrain.h"
#include "TestCube.h"
#include "Texture.h"
#include "Anubis.h"
#include "TestPlayer.h"

#include "FileIOMgr.h"
#include "WallCube.h"


IMPLEMENT_SINGLETON(CImGuiMgr)

ImGuiTextBuffer CImGuiMgr::log;

bool CImGuiMgr::Show_Terrain_Window = true;
bool CImGuiMgr::Show_Player_Window = false;
bool CImGuiMgr::Show_Main_Menu_Window = false;
bool CImGuiMgr::Show_Cube_Tool = false;
bool CImGuiMgr::Show_Monster_Tool = false;

_int CImGuiMgr::m_iInterval = 100;
_int CImGuiMgr::m_iWidth = 100;
_int CImGuiMgr::m_iDepth = 1;


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


	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 18)), E_FAIL);

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

void CImGuiMgr::TransformEdit(CCamera* pCamera, CTransform* pTransform, _bool& Window)
{
	if (true == Show_Cube_Tool)
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
		ImGuizmo::SetRect(lt.x, lt.y, io.DisplaySize.x, io.DisplaySize.y);

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
	if (true == Show_Monster_Tool)
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
		ImGuizmo::SetRect(lt.x, lt.y, io.DisplaySize.x, io.DisplaySize.y);

		// ImGuizmo::DrawGrid(m_pCam->GetView(), m_pCam->GetPrj(), matId, 100.f);

		ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

		pTransform->m_matWorld = matWorld;

		ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
		memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
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

void CImGuiMgr::WindowLayOut()
{
	static float f = 0.0f;
	static int counter = 0;
	static int RadioButton;
	int HHW = 0;
	int BIH = 1;
	int PJW = 2;
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Terrain_Tool", &Show_Terrain_Window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("PlayerTool", &Show_Player_Window);
	ImGui::Checkbox("Main_MenuTool", &Show_Main_Menu_Window);
	ImGui::Checkbox("Cube_Tool", &Show_Cube_Tool);
	ImGui::Checkbox("Monster_Tool", &Show_Monster_Tool);

	if (ImGui::Button("Clear"))
	{
		Show_Player_Window = false;
		Show_Terrain_Window = false;
		Show_Main_Menu_Window = false;
		Show_Cube_Tool = false;
		Show_Monster_Tool = false;
	}

	
	
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::BeginChild("USER");
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
	{
		switch (m_CurrentUser)
		{
		case USER_HHW:
			CFileIOMgr::GetInstance()->Save_FileData(pScene, L"TestLayer2", L"../../Data/", L"Stage1Map.dat", OBJ_CUBE);
			break;

		case USER_BIH:
			CFileIOMgr::GetInstance()->Save_FileData(pScene, L"TestLayer2", L"../../Data/", L"Stage2Map.dat", OBJ_CUBE);
			break;

		case USER_PJW:
			CFileIOMgr::GetInstance()->Save_FileData(pScene, L"TestLayer2", L"../../Data/", L"Stage3Map.dat", OBJ_CUBE);
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
				L"MapCubeLayer",
				L"../../Data/",
				L"Stage1Map.dat",
				L"TestCube",
				OBJ_CUBE);
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
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
		MyLayer->Delete_GameObject(m_CurrentSelectGameObjectObjKey.c_str());
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

	if (m_bCubeCreateCheck)
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

		/*	char* str = "Text";
			ImGui::InputText("Text", str);

	*/
	



		ImGui::Text("if double click Create Cube");
		if (ImGui::IsMouseDoubleClicked(0))
		{
			ImVec2 temp = ImGui::GetMousePos();
			CGameObject *pGameObject = nullptr;
			CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
		
			_tchar* test1 = new _tchar[20];

			wstring t = pObjectName + L"%d";
			wsprintfW(test1, t.c_str(), m_iIndex);
			MyLayer->AddNameList(test1);

			_bool	isUpcube = false;

			map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();
			CGameObject *pWallCube = nullptr;
			_int iCount = 0;

			for (auto iter = test.begin(); iter != test.end(); ++iter)
			{
				if(dynamic_cast<CWallCube*>(iter->second)->Set_SelectGizmo())
				{ 
					isUpcube = true;
					pWallCube = iter->second;
					iCount++;
				}
			}

			if (iCount > 1)
			{
				ImGui::End();
				return;
			}
			_vec2 vec2MousePos = { temp.x,temp.y };
			pGameObject = CWallCube::Create(pGrahicDev, &vec2MousePos);
			NULL_CHECK_RETURN(pGameObject, );
			static_cast<CWallCube*>(pGameObject)->Set_DrawTexIndex(m_iMapCubeIndex);
			NULL_CHECK_RETURN(pGameObject, );


			if (isUpcube)
			{
				CTransform* pCubeTrnasform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
				CTransform* pPreCubeTransform = dynamic_cast<CTransform*>(pWallCube->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
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
					pCubeTrnasform->Set_Pos(vPrePos.x, vPrePos.y + 1.f, vPrePos.z);
					break;
				case Engine::CREATECUBE_DOWN:
					pCubeTrnasform->Set_Pos(vPrePos.x, vPrePos.y - 1.f, vPrePos.z);
					break;
				case Engine::CREATECUBE_END:
					break;
				default:
					break;
				}
			}

			FAILED_CHECK_RETURN(MyLayer->Add_GameObject(test1, pGameObject), );

			++m_iIndex;
			pScene->Add_Layer(MyLayer, L"MapCubeLayer");
		}
	}

	if (m_bCubeSelcetCheck)
	{
		if (ImGui::IsMouseClicked(0))
		{
			CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
			map<const _tchar*, CGameObject*> CubeMap = MyLayer->Get_GameObjectMap();

			for (auto iter = CubeMap.begin(); iter != CubeMap.end(); ++iter)
			{

				if (dynamic_cast<CWallCube*>(iter->second)->Set_SelectGizmo())
				{
					pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
					m_CurrentSelectGameObjectObjKey = iter->first;
				}
			}
		}
	}

	CWallCube* pGameObject = dynamic_cast<CWallCube*>(Engine::Get_GameObject(L"MapCubeLayer", m_CurrentSelectGameObjectObjKey.c_str()));
	if (pGameObject)
	{
		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static _bool	bWireFrame = false;
			if (ImGui::Checkbox("WireFrame", &bWireFrame))
			{
				CLayer* MyLayer = pScene->GetLayer(L"MapCubeLayer");
				map<const _tchar*, CGameObject*> CubeMap = MyLayer->Get_GameObjectMap();
				
				for (auto iter = CubeMap.begin(); iter != CubeMap.end(); ++iter)
				{
					static_cast<CWallCube*>(iter->second)->Set_WireFrame(bWireFrame);
				}
			}
		}

			ImGui::NewLine();
			if (ImGui::CollapsingHeader("Tile Texture", ImGuiTreeNodeFlags_DefaultOpen))
			{
				CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_MapCubeTexture", ID_STATIC));

			vector<IDirect3DBaseTexture9*> vecTexture = pTextureCom->Get_Texture();

				for (_uint i = 0; i < vecTexture.size(); ++i)
				{
					if (ImGui::ImageButton((void*)vecTexture[i], ImVec2(60.f, 60.f)))
					{
						m_iMapCubeIndex = i;
						pGameObject->Set_DrawTexIndex(m_iMapCubeIndex);
					}
					if (i == 0 || (i + 1) % 6)
						ImGui::SameLine();
				}
			}
	}
	else
	{
		ImGui::End();
		return;
	}

	TransformEdit(pCam, m_pSelectedTransform, Show_Cube_Tool);
	// ������ư�� ���Ѱ�
	if (pTranscom != nullptr)
		m_pSelectedTransform = pTranscom;

	ImGui::End();
}

void CImGuiMgr::TerrainTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene)
{
	if (!Show_Terrain_Window)
		return;

	ImGui::Begin("Terrain Settings");

	if (ImGui::CollapsingHeader("Tile Count", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderInt("Width", &m_iWidth, 0, 200);
		ImGui::SliderInt("Depth", &m_iDepth, 0, 200);
		ImGui::SliderInt("Interval", &m_iInterval, 0, 10);
		ImGui::NewLine();
		if (ImGui::Button("Create"))
		{
			CLayer* MyLayer = pScene->GetLayer(L"TestLayer");
			MyLayer->Delete_GameObject(L"TestMap");

			Engine::Delete_Proto(L"Proto_TerrainTexCom");
			FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(pGrahicDev, m_iWidth, m_iDepth, m_iInterval)), );

			CGameObject* pGameObject = nullptr;

			pGameObject = CTerrain::Create(pGrahicDev);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(MyLayer->Add_GameObject(L"TestMap", pGameObject), );

			ImGui::SameLine();
			if (ImGui::Button("Clear"))
			{
				CLayer* pLayer = pScene->GetLayer(L"TestLayer");
				pLayer->Delete_GameObject(L"TestMap");
			}
		}
	}

	CTerrain* pGameObject = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"TestLayer", L"TestMap"));
	if (pGameObject)
	{
		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static _bool	bWireFrame = false;
			if (ImGui::Checkbox("WireFrame", &bWireFrame))
				pGameObject->Set_WireFrame(bWireFrame);
		}
		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Tile Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_TerrainTexture2", ID_STATIC));

			vector<IDirect3DBaseTexture9*> vecTexture = pTextureCom->Get_Texture();

			for (_uint i = 0; i < vecTexture.size(); ++i)
			{
				if (ImGui::ImageButton((void*)vecTexture[i], ImVec2(32.f, 32.f)))
				{
					//pTextureCom->Set_Texture(i);
					pGameObject->m_iTerrainIdx = i;
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
		}
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
		//�ϴ� ���̺� �ε� ���� ���� ���� ����

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

		CTransform * pTranscom = nullptr;
		if (m_bMonsterCreateCheck)
		{
			ImGui::Text("if double click Create Cube");
			if (ImGui::IsMouseDoubleClicked(0))
			{
				ImVec2 temp = ImGui::GetMousePos();

				//_vec3 vTemp = temp.x
				CGameObject *pGameObject = nullptr;

				_tchar* test1 = new _tchar[20];

				wstring t = L"Test%d";
				wsprintfW(test1, t.c_str(), m_iIndex);

				NameList.push_back(test1);

				pGameObject = CAnubis::Create(pGrahicDev, temp.x, temp.y);
				NULL_CHECK_RETURN(pGameObject, );

				CLayer* pMonsterlayer = pScene->GetLayer(L"TestLayer3");

				FAILED_CHECK_RETURN(pMonsterlayer->Add_GameObject(test1, pGameObject), );

				++m_iIndex;

				pScene->Add_Layer(pMonsterlayer, L"TestLayer3");
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
					if (dynamic_cast<CMonsterBase*>(iter->second)->Set_SelectGizmo(g_hWnd, static_cast<CAnubis*>(iter->second)->Get_Calculator(), static_cast<CAnubis*>(iter->second)->Get_Buffer()))
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
		}
		CGameObject* pGameObject = dynamic_cast<CMonsterBase*>(Engine::Get_GameObject(L"TestLayer3", m_CurrentSelectGameObjectObjKey.c_str()));

		ImGui::NewLine();
		//����Ʈ �� ���� ���� Ȥ�� �̸� �����ؼ� create �ؾ���
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
		// ������ư�� ���Ѱ�
		if (pTranscom != nullptr)
			m_pSelectedTransform = pTranscom;

		ImGui::End();
		if (pGameObject != nullptr)
		{
			CharacterInfo* monInfo = nullptr;
			monInfo = &(static_cast<CMonsterBase*>(pGameObject)->Get_InfoRef());
			ImGui::Begin("Monster Stat");

			ImGui::Text("Monster Stat Setting Window");
			ImGui::InputInt("Hp", &monInfo->_iHp);
			ImGui::InputInt("AttackPower", &monInfo->_iAttackPower);
			ImGui::InputInt("MonsterIndex", &(static_cast<CMonsterBase*>(pGameObject)->Get_MonsterType()));
			//������ �޾ƿ��� ������ �� �ִ� ����� ������ ��

			ImGui::End();
		}
	}
}


// Player Tool
void CImGuiMgr::Player_Tool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eId)
{
	if (!Show_Player_Window)
		return;

	ImGui::Begin("Player Setting Manager");

	CTestPlayer* pGameObject = dynamic_cast<CTestPlayer*>(Engine::Get_GameObject(L"TestLayer", L"TestPlayer"));

	if (pGameObject)
	{
		if (ImGui::CollapsingHeader("Player Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_PlayerTexture2", ID_STATIC));

			//CTransform* pTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

			vector<IDirect3DBaseTexture9*> vecPlayerTexture = pTextureCom->Get_Texture();

			for (_uint i = 0; i < 5; ++i)
			{
				if (ImGui::ImageButton((void*)vecPlayerTexture[i], ImVec2(40.f, 40.f)))
				{
					pGameObject->Set_DrawTexIndex(i);
				}
				if (i == 0 || (i + 1) % 3)
					ImGui::SameLine();
			}
		}

		ImGui::Text("this is Transform_ButtonMenu");
		if (ImGui::Button("Save"))
		{
			//CFileIOMgr::GetInstance()->Save_FileData(pScene, L"TestLayer2", L"../../Data/", L"TESp.dat", OBJ_CUBE);
			CFileIOMgr::GetInstance()->Save_FileData(pScene, pLayerTag, L"../../Data/", L"Player.dat", OBJ_PLAYER);
			//Save_Obj_Transform(pScene, pDirectory, pLayerTag, pObjTag, pComponentTag, eId);

		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			//Load_Obj_Transform(pGraphicDev, pScene, pDirectory, pLayerTag, pComponentTag, eId, PlayerList, pObjTag);

			/*CFileIOMgr::GetInstance()->Load_FileData(pGrahicDev,
			pScene,
			L"TestLayer2",
			L"../../Data/",
			L"TESp.dat",
			L"TestCube",
			OBJ_CUBE);*/

			CFileIOMgr::GetInstance()->Load_FileData(pGraphicDev,
				pScene,
				const_cast<_tchar*>(pLayerTag),
				L"../../Data/",
				L"Player.dat",
				L"TestPlayer",
				OBJ_PLAYER);


		}
	}
	ImGui::End();

}

// Stage -> this
//void CImGuiMgr::Save_Obj_Transform(CScene * pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
//{						// L"../../Data/Map.dat"
//	wstring Directory = pDirectory;
//
//	HANDLE      hFile = CreateFile(Directory.c_str(),
//		// ������ ��ο� �̸�
//		GENERIC_WRITE,         // ���� ���� ��� (GENERIC_WRITE : ���� ����, GENERIC_READ : �б� ����)
//		NULL,               // ���� ���(������ �����ִ� ���¿��� �ٸ� ���μ����� ������ �� ����� ���ΰ�)    
//		NULL,               // ���� �Ӽ�(NULL�� �����ϸ� �⺻�� ����)
//		CREATE_ALWAYS,         // CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING  : ������ ���� ��쿡�� ����
//		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�(�б� ����, ���� ��) : FILE_ATTRIBUTE_NORMAL : �ƹ��� �Ӽ��� ���� ����
//		NULL);               // ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��ϱ� NULL)
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		return;
//	}
//	// L"TestLayer2" 
//	CLayer* MyLayer = pScene->GetLayer(pLayerTag);
//	DWORD   dwByte = 0;
//
//	map<const _tchar*, CGameObject*> map_SearchObj = MyLayer->Get_GameObjectMap();
//	auto iter = find_if(map_SearchObj.begin(), map_SearchObj.end(), CTag_Finder(pObjTag));
//
//	// L"Proto_TransformCom", ID_DYNAMIC
//	CTransform* Transcom = dynamic_cast<CTransform*>(iter->second->Get_Component(pComponentTag, eID));
//
//	_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
//	_int	iDrawNum = 0;
//
//	Transcom->Get_Info(INFO_RIGHT, &vRight);
//	Transcom->Get_Info(INFO_UP, &vUp);
//	Transcom->Get_Info(INFO_LOOK, &vLook);
//	Transcom->Get_Info(INFO_POS, &vPos);
//	memcpy(vScale, Transcom->m_vScale, sizeof(_vec3));
//	memcpy(vAngle, Transcom->m_vAngle, sizeof(_vec3));
//	iDrawNum = iter->second->Get_DrawTexIndex();
//
//	WriteFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
//	WriteFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
//	WriteFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
//	WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
//	WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
//	WriteFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
//	WriteFile(hFile, &iDrawNum, sizeof(_int), &dwByte, nullptr);
//
//
//
//	CloseHandle(hFile);
//	MSG_BOX("Save_Complete");
//
//}
//// m_pGraphicDev	// Stage -> this
//void CImGuiMgr::Load_Obj_Transform(LPDIRECT3DDEVICE9 pGrahicDev, CScene * pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar * pComponentTag, COMPONENTID eId, list<_tchar*> pList, const _tchar* pObjTag)
//{						// L"../../Data/Map.dat";
//	wstring Directory = pDirectory;
//
//	HANDLE      hFile = CreateFile(Directory.c_str(),      // ������ ��ο� �̸�
//		GENERIC_READ,         // ���� ���� ��� (GENERIC_WRITE : ���� ����, GENERIC_READ : �б� ����)
//		NULL,               // ���� ���(������ �����ִ� ���¿��� �ٸ� ���μ����� ������ �� ����� ���ΰ�)    
//		NULL,               // ���� �Ӽ�(NULL�� �����ϸ� �⺻�� ����)
//		OPEN_EXISTING,         // CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING  : ������ ���� ��쿡�� ����
//		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�(�б� ����, ���� ��) : FILE_ATTRIBUTE_NORMAL : �ƹ��� �Ӽ��� ���� ����
//		NULL);               // ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��ϱ� NULL)
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		return;
//	}
//
//	DWORD   dwByte = 0;
//
//	_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
//	_int	iDrawIndex = 0;
//	CLayer* pMyLayer = nullptr;
//
//
//	ReadFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
//	ReadFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
//	ReadFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
//	ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
//	ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
//	ReadFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
//	ReadFile(hFile, &iDrawIndex, sizeof(_int), &dwByte, nullptr);
//	
//	pMyLayer = pScene->GetLayer(pLayerTag);
//	
//	CGameObject *pGameObject = nullptr;
//	_tchar* szObjName = new _tchar[20];
//	szObjName = L"TestPlayer";
//
//	//pList.push_back(szObjName);
//	pMyLayer->AddNameList(szObjName);
//
//	pGameObject = CTestPlayer::Create(pGrahicDev);
//	
//
//	FAILED_CHECK_RETURN(pMyLayer->Delete_GameObject(pObjTag));
//	FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(szObjName, pGameObject), );
//	pGameObject->Set_DrawTexIndex(iDrawIndex);
//	++m_iIndex;
//
//	CTransform* Transcom = dynamic_cast<CTransform*>(pGameObject->Get_Component(pComponentTag, eId));
//
//	Transcom->Set_Info(INFO_RIGHT, &vRight);
//	Transcom->Set_Info(INFO_UP, &vUp);
//	Transcom->Set_Info(INFO_LOOK, &vLook);
//	Transcom->Set_Info(INFO_POS, &vPos);
//	Transcom->Set_Angle(&vAngle);
//	Transcom->Set_Scale(&vScale);
//
//	Transcom->Update_Component(0.01f);
//
//
//	MSG_BOX("Save_Complete");
//	pScene->Add_Layer2(pMyLayer, pLayerTag);
//
//
//	CloseHandle(hFile);
//
//}



void CImGuiMgr::Free()
{
	for (auto iter : PlayerList)
	{
		Safe_Delete_Array(iter);
	}
}