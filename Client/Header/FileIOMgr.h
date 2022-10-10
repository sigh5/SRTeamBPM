#pragma once
#include "Base.h"
#include "Engine_Include.h"
#include "Scene.h"



class CFileIOMgr : public CBase
{
	DECLARE_SINGLETON(CFileIOMgr)


private:
	explicit CFileIOMgr();
	virtual ~CFileIOMgr();


public:
	void Save_FileData(CScene * pScene, wstring LayerName, wstring FilePath, wstring FileName, OBJ_TYPE eObjType);
	void Load_FileData(LPDIRECT3DDEVICE9 pGrahicDev,
		CScene *pScene, 
		_tchar* LayerName,
		wstring filePath, 
		wstring FileName,
		wstring pObjectName,
		OBJ_TYPE eObjType);

	//ÁÖ¼®Áö¿ì¼À
	
public:
	_int		Get_Index() { return m_iIndex; }

private:
	virtual void Free()override;

	_int		m_iIndex = 0;		// Texture Index

};

