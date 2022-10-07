#pragma once
#include "Base.h"
#include "Engine_Include.h"
#include "ImGuiFileBrowser.h"
#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"


BEGIN(Engine)

class CCamera;
class CTransform;
class CScene;
class CLayer;
class CGameObject;
END

class CWallCube;
class CTerrain;

// TexturePath 를 위한 Struct
struct TexturePath
{
	PDIRECT3DTEXTURE9 pTexture;
	_tchar*	wszFilePathName;
};

class CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

private:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr();

public:
	// 예시
	static void TransformEdit(CCamera* pCamera, CTransform* pTransform, _bool& Window); // Gizmo
	static void LoggerWindow();
	static void LogOut(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		log.appendfv(fmt, args);
		va_end(args);
	}
	// 예시 

public:
	HRESULT Ready_MapTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene);	//  맵툴에 필요한 리소스를 생성합니다.
	

	HRESULT Ready_PlayerTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene);	// 플레이어 툴
	HRESULT Ready_MonsterTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene);   // 몬스터 툴

public: // Terrain 
	void  TerrainTool(LPDIRECT3DDEVICE9 pGrahicDev, CCamera* pCam, CScene* pScene);	// Terrain Tool
	
	


public: // Wall
	_int	CheckCubeinPicking(CLayer* pLayer, CWallCube** pWallCube,_bool* bUp);
	
	void	Set_CubeDir();
	void	Set_All_CubeHeight(CLayer* pLayer);
	
	void	Set_Create_Cube_Pos(CGameObject** pGameObject, CWallCube** pWallCube,_bool* isUpcube);
	void	Set_Cube_WireFrame(CLayer* pLayer);


	
	void	Save_CubeMap(CScene *pScene);
	void	Load_CubeMap(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene);




public:
	// Common
	template <typename T>
	void  ObjectCreate(LPDIRECT3DDEVICE9 pGrahicDev, CLayer* pLayer, CGameObject** pGameObject,const wstring& objStr);
	template <typename T>
	CGameObject*   SelectObject(CLayer* pLayer, wstring* currentObjectName);
	template <typename T>
	HRESULT  EditObjectTexture(const wstring& m_CureentTextureProtoName);

	




	CGameObject*	m_pSelectedObject=nullptr;
	CTransform*		m_pSelectedTransform = nullptr;
	
	wstring			m_CurrentObjectName = L"";
	wstring         m_CurrentTerrainObjectName = L"";
	//wstring			m_CureentTextureProtoName = L"";

public:
	// Map_Tool 기능 생성
	// 창생성
	void  WindowLayOut(); // 맨처음 버튼
	void  CreateObject(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera *pCam, wstring pObjectName);// 오브젝트 생성및 저장하고 불러오기																			
	void  MonsterTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera *pCam); // ~Map_Tool 기능 생성
	void  Player_Tool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eId);   // // 09.25 PJW 추가 작업

private:
	USER_TYPE	m_CurrentUser;
	//사용자 구분용

	// Map_Tool 변수
	wstring	m_CurrentSelectGameObjectObjKey = L"";
	_bool	m_bCubeCreateCheck = false;
	_bool	m_bCubeSelcetCheck = false;
	_int		m_iIndex = 0;

	static _int m_iWidth;
	static _int m_iDepth;
	static _int m_iInterval;

	list<_tchar* > NameList;
	_int			m_iMapCubeIndex = 0;

	
	CWallCube*	m_pWallCube = nullptr;
	


	// ~Map_Tool 변수

	// Player Tool 변수
	list<_tchar* > PlayerList;
	// ~Player Tool 변수

	// Monster_Tool 변수
	_bool	m_bMonsterCreateCheck = false;
	_bool	m_bMonsterSelectCheck = false;

public:
	//  window창 생성시 필요한 불변수
	static  bool Show_Terrain_Window;
	static	bool Show_Player_Window;
	static  bool Show_Main_Menu_Window;
	static  bool Show_Cube_Tool;
	static	bool Show_Monster_Tool;
	static ImVec4 clear_color;
	// ~ window창 생성시 필요한 불변수

	// File 변수
	static ImGuiFileDialog fileDialog;
	static imgui_addons::ImGuiFileBrowser file_dialog;
	// ~File 변수

private:
	// 예시
	static ImGuiTextBuffer log;
	CREATECUBE cubePlane = CREATECUBE_END;
	virtual void Free() override;
	// ~예시
};


template<typename T>
void CImGuiMgr::ObjectCreate(LPDIRECT3DDEVICE9 pGrahicDev, CLayer* pLayer, CGameObject** pGameObject,const wstring& objStr)
{
	static _int iObjectIndex = 0;

	_tchar* szObjectName = new _tchar[20];
	wstring t = objStr + L"%d";

	wsprintfW(szObjectName, t.c_str(), iObjectIndex);
	pLayer->AddNameList(szObjectName);
	++iObjectIndex;

	*pGameObject = T::Create(pGrahicDev);
	NULL_CHECK_RETURN(*pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjectName, *pGameObject), );

}

template<typename T>
CGameObject* CImGuiMgr::SelectObject(CLayer* pLayer,wstring* currentObjectName)
{

	map<const _tchar*, CGameObject*> MapLayer = pLayer->Get_GameObjectMap();

	for (auto iter = MapLayer.begin(); iter != MapLayer.end(); ++iter)
	{
		if (dynamic_cast<T*>(iter->second)->Set_SelectGizmo())
		{
			m_pSelectedTransform = dynamic_cast<CTransform*>(iter->second->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
			m_pSelectedObject = dynamic_cast<T*>(iter->second);
			m_CurrentObjectName = iter->first;
			
			*currentObjectName = iter->first;


			return m_pSelectedObject;
		}
	}

	
	if (m_pSelectedObject != nullptr)
		return m_pSelectedObject;

}

template<typename T>
HRESULT CImGuiMgr::EditObjectTexture(const wstring& m_CureentTextureProtoName)
{
	if (m_pSelectedObject)
	{
		//ImGui::NewLine();
		//if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
		//{
		//	static _bool	bWireFrame = false;
		//	/*	if (ImGui::Checkbox("WireFrame", &bWireFrame))
		//	static_cast<T>(m_pSelectedObject)->Set_WireFrame(bWireFrame);*/
		//}

		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Tile Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			CTexture* pTextureCom = dynamic_cast<CTexture*>(dynamic_cast<T*>(m_pSelectedObject)->Get_Component(m_CureentTextureProtoName.c_str(), ID_STATIC));
			NULL_CHECK_RETURN(pTextureCom, E_FAIL);

			vector<IDirect3DBaseTexture9*> vecTexture = pTextureCom->Get_Texture();

			for (_uint i = 0; i < vecTexture.size(); ++i)
			{
				if (ImGui::ImageButton((void*)vecTexture[i], ImVec2(32.f, 32.f)))
				{
					m_pSelectedObject->Set_DrawTexIndex(i);
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
		}
	}

	return S_OK;

}