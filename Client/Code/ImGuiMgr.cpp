#include "stdafx.h"
#include "ImGuiMgr.h"
#include "ImGuizmo.h"
#include "imgui.h"

#include "Export_Function.h"
#include "Terrain.h"
#include "TestCube.h"
#include "Texture.h"
IMPLEMENT_SINGLETON(CImGuiMgr)

ImGuiTextBuffer CImGuiMgr::log;

bool CImGuiMgr::Show_Terrain_Window = true;
bool CImGuiMgr::Show_Player_Window = false;
bool CImGuiMgr::Show_Main_Menu_Window = false;
bool CImGuiMgr::Show_Cube_Tool = false;

_int CImGuiMgr::m_iInterval = 100;
_int CImGuiMgr::m_iWidth = 100;
_int CImGuiMgr::m_iDepth = 1;

ImVec4 CImGuiMgr::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

CImGuiMgr::CImGuiMgr()
{
	
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
	pScene->Add_Layer(pLayer, L"TestLayer2");


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 18)), E_FAIL);

	return S_OK;
}

HRESULT CImGuiMgr::Ready_PlayerTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene)
{
	return S_OK;
}

HRESULT CImGuiMgr::Ready_MonsterTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene)
{
	return S_OK;
}

void CImGuiMgr::TransformEdit(CCamera* pCamera, CTransform* pTransform, _bool& Window)
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
	memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
	memcpy(&pTransform->m_vAngle, matrixRotation, sizeof(matrixRotation));
	memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));




	ImGui::End();
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
	// if (ImGui::Button("Add 1000 lines"))
	// {
	//     for (int i = 0; i < 1000; i++)
	//         log.appendf("%i The quick brown fox jumps over the lazy dog\n", lines + i);
	//     lines += 1000;
	// }
	ImGui::BeginChild("Log");
	switch (test_type)
	{
	case 0:
		// Single call to TextUnformatted() with a big buffer
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

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Terrain_Tool", &Show_Terrain_Window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("PlayerTool", &Show_Player_Window);
	ImGui::Checkbox("Main_MenuTool", &Show_Main_Menu_Window);
	ImGui::Checkbox("Cube_Tool", &Show_Cube_Tool);

	if (ImGui::Button("Clear"))
	{
		Show_Player_Window = false;
		Show_Terrain_Window = false;
		Show_Main_Menu_Window = false;
		Show_Cube_Tool = false;
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void CImGuiMgr::CreateObject(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera* pCam)
{
	if (!Show_Cube_Tool)
		return;

	ImGui::Begin("Cube Settings");

	ImGui::Text("this is Transform_ButtonMenu");
	if (ImGui::Button("Save"))
	{
		Save_Transform(pScene);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		Load_Transform(pGrahicDev,pScene);
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		CLayer* MyLayer = pScene->GetLayer(L"TestLayer2");
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
		ImGui::Text("if double click Create Cube");
		if (ImGui::IsMouseDoubleClicked(0))
		{
			ImVec2 temp = ImGui::GetMousePos();
			CGameObject *pGameObject = nullptr;

			_tchar* test1 = new _tchar[20];

			wstring t = L"Test%d";
			wsprintfW(test1, t.c_str(), m_iIndex);

			NameList.push_back(test1);

			pGameObject = CTestCube::Create(pGrahicDev, temp.x, temp.y);
			NULL_CHECK_RETURN(pGameObject, );
			
			CLayer* pCubelayer = pScene->GetLayer(L"TestLayer2");

			FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(test1, pGameObject), );

			++m_iIndex;

			pScene->Add_Layer(pCubelayer, L"TestLayer2");
		}
	}

	if (m_bCubeSelcetCheck)
	{
		if (ImGui::IsMouseClicked(0))
		{
			CLayer* MyLayer = pScene->GetLayer(L"TestLayer2");

			map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

			for (auto iter = test.begin(); iter != test.end(); ++iter)
			{
				if (dynamic_cast<CTestCube*>(iter->second)->Set_SelectGizmo())
				{
					pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
					m_CurrentSelectGameObjectObjKey = iter->first;
				}
			}
		}
	}

	CTestCube* pGameObject = dynamic_cast<CTestCube*>(Engine::Get_GameObject(L"TestLayer2", m_CurrentSelectGameObjectObjKey.c_str()));
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
				CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_CubeTexture", ID_STATIC));

				vector<IDirect3DBaseTexture9*> vecTexture = pTextureCom->Get_Texture();

				for (_uint i = 0; i < 4; ++i)
				{
					if (ImGui::ImageButton((void*)vecTexture[i], ImVec2(60.f, 60.f)))
					{
						pGameObject->Set_DrawTexIndex(i);
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
	// 셀렉버튼을 위한것
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

			for (_uint i = 0; i < 18; ++i)
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

void CImGuiMgr::Save_Transform(CScene* pScene)
{
	wstring Directory = L"../../Data/Map.dat";

	HANDLE      hFile = CreateFile(Directory.c_str(),
		// 파일의 경로와 이름
		GENERIC_WRITE,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		CREATE_ALWAYS,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	CLayer* MyLayer = pScene->GetLayer(L"TestLayer2");
	DWORD   dwByte = 0;
	
	map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();
	for (auto iter = test.begin(); iter != test.end(); ++iter)
	{

		CTransform* Transcom = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
		_int	iDrawNum = 0;

		Transcom->Get_Info(INFO_RIGHT, &vRight);
		Transcom->Get_Info(INFO_UP, &vUp);
		Transcom->Get_Info(INFO_LOOK, &vLook);
		Transcom->Get_Info(INFO_POS, &vPos);
		memcpy(vScale, Transcom->m_vScale, sizeof(_vec3));
		memcpy(vAngle, Transcom->m_vAngle, sizeof(_vec3));
		iDrawNum = iter->second->Get_DrawTexIndex();

		WriteFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &iDrawNum, sizeof(_int), &dwByte, nullptr);

	}

	CloseHandle(hFile);
	MSG_BOX("Save_Complete");

}

void CImGuiMgr::Load_Transform(LPDIRECT3DDEVICE9 pGrahicDev,CScene *pScene)
{
	wstring Directory = L"../../Data/Map.dat";

	HANDLE      hFile = CreateFile(Directory.c_str(),      // 파일의 경로와 이름
		GENERIC_READ,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		OPEN_EXISTING,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD   dwByte = 0;

	_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
	_int	iDrawIndex = 0;
	CLayer* pMyLayer = nullptr;

	while (true)
	{
		ReadFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &iDrawIndex, sizeof(_int), &dwByte, nullptr);

		CGameObject *pGameObject = nullptr;
		_tchar* test1 = new _tchar[20];
		wstring t = L"Test%d";
		wsprintfW(test1, t.c_str(), m_iIndex);
		NameList.push_back(test1);
		
		pGameObject = CTestCube::Create(pGrahicDev);
		pMyLayer = pScene->GetLayer(L"TestLayer2");
		
		FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(test1, pGameObject), );
		pGameObject->Set_DrawTexIndex(iDrawIndex);
		++m_iIndex;

		CTransform* Transcom = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

		Transcom->Set_Info(INFO_RIGHT, &vRight);
		Transcom->Set_Info(INFO_UP, &vUp);
		Transcom->Set_Info(INFO_LOOK, &vLook);
		Transcom->Set_Info(INFO_POS, &vPos);
		Transcom->Set_Angle(&vAngle);
		Transcom->Set_Scale(&vScale);

		Transcom->Update_Component(0.01f);

		//   받아온 정보 입력해줘야함

		if (0 == dwByte)
			break;

	}
	MSG_BOX("Save_Complete");
	pScene->Add_Layer(pMyLayer, L"TestLayer2");


	CloseHandle(hFile);



}

void CImGuiMgr::Free()
{
	//Release();
	
}