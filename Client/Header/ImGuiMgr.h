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
	void  CreateObject(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene,CCamera *pCam, wstring pObjectName);// 오브젝트 생성
	void  TerrainTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene);	// Terrain Tool

	// 범용적으로 뺌
	//void  Save_Transform(CScene* pScene);	// 큐브 오브젝트 저장
	//void  Load_Transform(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene);// 큐브 오브젝트 로드

	// ~Map_Tool 기능 생성
	void  MonsterTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera *pCam);
	void  Save_Monster(CScene* pScene);
	void  Load_Monster(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene);
	void  TransformEdit_Monster(CCamera* pCamera, CTransform* pTransform, _bool& Window);
	// Monster_Tool 기능 생성

	// 각툴의 기능 쓰기




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


