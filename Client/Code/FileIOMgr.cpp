#include "stdafx.h"
#include "FileIOMgr.h"
#include "Layer.h"

#include "Export_Function.h"
#include "TestCube.h"

IMPLEMENT_SINGLETON(CFileIOMgr)

CFileIOMgr::CFileIOMgr()
{
}


CFileIOMgr::~CFileIOMgr()
{
}

void CFileIOMgr::Save_FileData(CScene * pScene,
	wstring LayerName, 
	wstring FilePath,
	wstring FileName,
	OBJ_TYPE eObjType)
{
	wstring Directory = FilePath + FileName;


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

	CLayer* MyLayer = pScene->GetLayer(LayerName.c_str());
	DWORD   dwByte = 0;
	map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();
	switch (eObjType)
	{
	case Engine::OBJ_CUBE:
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
		break;

	case Engine::OBJ_PLAYER:
		break;
	case Engine::OBJ_MONSTER:
		break;
	case Engine::OBJ_TYPE_END:
		break;
	default:
		break;
	}
	

	CloseHandle(hFile);
	MSG_BOX("Save_Complete");

}

void CFileIOMgr::Load_FileData(LPDIRECT3DDEVICE9 pGrahicDev,
	CScene *pScene,
	_tchar* LayerName,
	wstring filePath,
	wstring FileName,
	wstring pObjectName,
	OBJ_TYPE eObjType)
{
	wstring Directory = filePath + FileName;

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
	pMyLayer = pScene->GetLayer(LayerName);
	_int	 iIndex = 0;

	switch (eObjType)
	{
	case Engine::OBJ_CUBE:
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

			wstring t = pObjectName + L"%d";
			wsprintfW(test1, t.c_str(), iIndex);

			pMyLayer->AddNameList(test1);

			++iIndex;

			pGameObject = CTestCube::Create(pGrahicDev);
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(test1, pGameObject), );
			pGameObject->Set_DrawTexIndex(iDrawIndex);

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
		break;
	case Engine::OBJ_PLAYER:
		// exForPlayerLoad
		break;
	case Engine::OBJ_MONSTER:
		// exForMONSTERLoad
		break;
	case Engine::OBJ_TYPE_END:
		break;
	default:
		break;
	}
	MSG_BOX("Save_Complete");
	pScene->Add_Layer(pMyLayer, LayerName);


	CloseHandle(hFile);
}



void CFileIOMgr::Free()
{
}
