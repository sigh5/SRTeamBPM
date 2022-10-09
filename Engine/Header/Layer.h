#pragma once

#include "GameObject.h"
#include "Engine_Include.h"
#include <stack>
BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*				Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*			Get_GameObject(const _tchar * pObjTag);

	// PJW insert code : 작업하면서 쓸거 같아서 만들었어요.
	map<const _tchar*, CGameObject*>			Get_MapObject() { return m_mapObject; } // map 접근용
	// ~PJW insert code

public:
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pInstance);
	HRESULT			Add_GameObjectList(CGameObject* pInstance);
	void			Add_GhulList(CGameObject* pGhul);
	void			Add_ObeliskList(CGameObject* pObelisk);

	
	
	HRESULT			Ready_Layer(void);
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(void);

	HRESULT Delete_GameObject(const _tchar * pObjTag);
	map<const _tchar*, CGameObject*>& Get_GameObjectMap()  { return m_mapObject; }

	// 맵오류때문에 키값으로 찾게 하는코드
	list<_tchar* > Get_NameList() { return NameList; }
	void AddNameList(_tchar* name) { NameList.push_back(name); }
	// ~맵오류때문에 키값으로 찾게 하는코드


	list<CGameObject*> *	Get_TeleCubeList(_int iIndex) { return &m_TeleportCubeList[iIndex]; }
	vector<CGameObject*>*	GetRestCube() { return &m_RestCubevec; }

	void					initStartCube();
	void					Delete_Current_Room(_int iIndex){m_RestCubevec.erase(m_RestCubevec.begin() + iIndex);} //방문할 큐브목록 삭제
	void					Save_CurrentRoom(CGameObject* pGameObject){m_vecVistedCube.push(pGameObject);} // 스택에 현재 방에서 나머지 갈방들을 삭제

	CGameObject*			Get_PreRoomTeleCube(); // 스택안의 top을 가져옴
	void					Clear_Stack();	// 스택 안에있는 것들을 지워줌


	
	//구울 리스트 가져오기
	list<CGameObject*>&		Get_GhulList() { return m_GhulList; }
	list<CGameObject*>&		Get_ObeliskList() { return m_ObeliskList; }

private:
	map<const _tchar*, CGameObject*>			m_mapObject;

	list<CGameObject*> m_TeleportCubeList[TELEPORT_CUBE_LIST_END];
	vector<CGameObject*> m_RestCubevec;		// 방문할예정인 큐브들
	stack<CGameObject*> m_vecVistedCube;	// 방문한 큐브들
											// 위에 3개의 자료형들은 삭제만 해줘야됌
											// 안에있는것들을 지우면 안됌

	list<CGameObject*>  m_GhulList;
	list<CGameObject*>  m_objPoolList;
	list<CGameObject*>	m_ObeliskList;

	list<_tchar* > NameList;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);


	_int				m_iRoomIndex = 0;		// 현재방의 인덱스
	_int				m_iRestRoom = 5;		// 나머지 방의수
};

END