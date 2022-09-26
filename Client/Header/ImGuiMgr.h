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
// TexturePath �� ���� Struct
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
	// ����
	static void TransformEdit(CCamera* pCamera, CTransform* pTransform, _bool& Window); // Gizmo
	static void LoggerWindow();
	static void LogOut(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		log.appendfv(fmt, args);
		va_end(args);
	}
	// ���� 

public:
	HRESULT Ready_MapTool(LPDIRECT3DDEVICE9 pGraphicDev,CScene* pScene);	//  ������ �ʿ��� ���ҽ��� �����մϴ�.

	HRESULT Ready_PlayerTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene);	// �÷��̾� ��
	HRESULT Ready_MonsterTool(LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene);   // ���� ��



public: 
	// Map_Tool ��� ����
	// â����
	void  WindowLayOut(); // ��ó�� ��ư

	void  CreateObject(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene,CCamera *pCam, wstring pObjectName);// ������Ʈ ������ �����ϰ� �ҷ�����
	void  TerrainTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene);	// Terrain Tool

	// ~Map_Tool ��� ����
	void  MonsterTool(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera *pCam);
	//void  Save_Monster(CScene* pScene);
	//void  Load_Monster(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene);
	void  TransformEdit_Monster(CCamera* pCamera, CTransform* pTransform, _bool& Window);
	// Monster_Tool ��� ����

	// ������ ��� ����

	// 09.25 PJW �߰� �۾�
	void  Player_Tool(LPDIRECT3DDEVICE9 pGraphicDev, CScene * pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eId);   // 

	//void  Save_Obj_Transform(CScene* pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eID);   // ������Ʈ ���� (�Ű����� �޾Ƽ� ���)
	//void  Load_Obj_Transform(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene, wstring pDirectory, const _tchar* pLayerTag, const _tchar * pComponentTag, COMPONENTID eID, list<_tchar*> pList, const _tchar* pObjTag); // ������Ʈ �ҷ����� (�Ű����� �޾Ƽ� ���)
	// ~09.25 PJW �߰� �۾�



private:
	// Map_Tool ����
	wstring	m_CurrentSelectGameObjectObjKey = L"";
	_bool	m_bCubeCreateCheck = false;
	_bool	m_bCubeSelcetCheck = false;
	_int		m_iIndex = 0;
	
	static _int m_iWidth;
	static _int m_iDepth;
	static _int m_iInterval;

	list<_tchar* > NameList;

	CTransform* m_pSelectedTransform = nullptr;
	// ~Map_Tool ����

	// Player Tool ����
	list<_tchar* > PlayerList;
	// ~Player Tool ����

	// Monster_Tool ����
	_bool	m_bMonsterCreateCheck = false;
	_bool	m_bMonsterSelectCheck = false;

public:
	//  windowâ ������ �ʿ��� �Һ���
	static  bool Show_Terrain_Window;
	static	bool Show_Player_Window;
	static  bool Show_Main_Menu_Window;
	static  bool Show_Cube_Tool;
	static	bool Show_Monster_Tool;
	static ImVec4 clear_color;
	// ~ windowâ ������ �ʿ��� �Һ���
	
	// File ����
	static ImGuiFileDialog fileDialog;
	static imgui_addons::ImGuiFileBrowser file_dialog;
	// ~File ����

private:
	// ����
	static ImGuiTextBuffer log;
	CREATECUBE cubePlane = CREATECUBE_END;
	virtual void Free() override;
	// ~����
};


