#pragma once
#include "Base.h"
#include "Engine_Include.h"
#include "ImGuiFileBrowser.h"
#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"
#include "Scene.h"

BEGIN(Engine)

class CCamera;
class CTransform;


END
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
	HRESULT Ready_MapTool(LPDIRECT3DDEVICE9 pGraphicDev,CScene* pScene);	//  맵툴에 필요한 리소스를 생성합니다.

	HRESULT Ready_PlayerTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene);	// 플레이어 툴
	HRESULT Ready_MonsterTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene);   // 몬스터 툴



public: 
	// Map_Tool 기능 생성
	// 창생성
	void  WindowLayOut(); // 맨처음 버튼

	void  CreateObject(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene,CCamera *pCam, wstring pObjectName);// 오브젝트 생성및 저장하고 불러오기
	void  TerrainTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene);	// Terrain Tool

	// ~Map_Tool 기능 생성
	void  MonsterTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera *pCam);
	//void  Save_Monster(CScene* pScene);
	//void  Load_Monster(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene);
	
	// Monster_Tool 기능 생성

	// 각툴의 기능 쓰기

	// 09.25 PJW 추가 작업
	void  Player_Tool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eId);   // 

	//void  Save_Obj_Transform(CScene* pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eID);   // 오브젝트 저장 (매개변수 받아서 사용)
	//void  Load_Obj_Transform(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar * pComponentTag, COMPONENTID eID, list<_tchar*> pList, const _tchar* pObjTag); // 오브젝트 불러오기 (매개변수 받아서 사용)
	//// ~09.25 PJW 추가 작업



private:
	// Map_Tool 변수
	wstring	m_CurrentSelectGameObjectObjKey = L"";
	_bool	m_bCubeCreateCheck = false;
	_bool	m_bCubeSelcetCheck = false;
	_int		m_iIndex = 0;
	
	static _int m_iWidth;
	static _int m_iDepth;
	static _int m_iInterval;

	list<_tchar* > NameList;

	CTransform* m_pSelectedTransform = nullptr;
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


