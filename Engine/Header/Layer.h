#pragma once

#include "GameObject.h"
#include "Engine_Include.h"

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
	
	
	
	HRESULT			Ready_Layer(void);
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(void);

	HRESULT Delete_GameObject(const _tchar * pObjTag);
	map<const _tchar*, CGameObject*>& Get_GameObjectMap()  { return m_mapObject; }

	// 맵오류때문에 키값으로 찾게 하는코드
	list<_tchar* > Get_NameList() { return NameList; }
	void AddNameList(_tchar* name) { NameList.push_back(name); }
	// ~맵오류때문에 키값으로 찾게 하는코드

private:
	map<const _tchar*, CGameObject*>			m_mapObject;


	list<CGameObject*>  m_objPoolList;

	list<_tchar* > NameList;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);

};

END