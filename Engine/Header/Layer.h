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

	// PJW insert code : �۾��ϸ鼭 ���� ���Ƽ� ��������.
	map<const _tchar*, CGameObject*>			Get_MapObject() { return m_mapObject; } // map ���ٿ�
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

	// �ʿ��������� Ű������ ã�� �ϴ��ڵ�
	list<_tchar* > Get_NameList() { return NameList; }
	void AddNameList(_tchar* name) { NameList.push_back(name); }
	// ~�ʿ��������� Ű������ ã�� �ϴ��ڵ�


	list<CGameObject*> *	Get_TeleCubeList(_int iIndex) { return &m_TeleportCubeList[iIndex]; }
	vector<CGameObject*>*	GetRestCube() { return &m_RestCubevec; }

	void					initStartCube();
	void					Delete_Current_Room(_int iIndex){m_RestCubevec.erase(m_RestCubevec.begin() + iIndex);} //�湮�� ť���� ����
	void					Save_CurrentRoom(CGameObject* pGameObject){m_vecVistedCube.push(pGameObject);} // ���ÿ� ���� �濡�� ������ ������� ����

	CGameObject*			Get_PreRoomTeleCube(); // ���þ��� top�� ������
	void					Clear_Stack();	// ���� �ȿ��ִ� �͵��� ������


	
	//���� ����Ʈ ��������
	list<CGameObject*>&		Get_GhulList() { return m_GhulList; }
	list<CGameObject*>&		Get_ObeliskList() { return m_ObeliskList; }

private:
	map<const _tchar*, CGameObject*>			m_mapObject;

	list<CGameObject*> m_TeleportCubeList[TELEPORT_CUBE_LIST_END];
	vector<CGameObject*> m_RestCubevec;		// �湮�ҿ����� ť���
	stack<CGameObject*> m_vecVistedCube;	// �湮�� ť���
											// ���� 3���� �ڷ������� ������ ����߉�
											// �ȿ��ִ°͵��� ����� �ȉ�

	list<CGameObject*>  m_GhulList;
	list<CGameObject*>  m_objPoolList;
	list<CGameObject*>	m_ObeliskList;

	list<_tchar* > NameList;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);


	_int				m_iRoomIndex = 0;		// ������� �ε���
	_int				m_iRestRoom = 5;		// ������ ���Ǽ�
};

END