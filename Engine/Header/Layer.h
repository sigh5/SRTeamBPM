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

	// PJW insert code : �۾��ϸ鼭 ���� ���Ƽ� ��������.
	map<const _tchar*, CGameObject*>			Get_MapObject() { return m_mapObject; } // map ���ٿ�
	// ~PJW insert code

public:
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pInstance);
	HRESULT			Add_GameObjectList(CGameObject* pInstance);
	
	
	
	HRESULT			Ready_Layer(void);
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(void);

	HRESULT Delete_GameObject(const _tchar * pObjTag);
	map<const _tchar*, CGameObject*>& Get_GameObjectMap()  { return m_mapObject; }

	// �ʿ��������� Ű������ ã�� �ϴ��ڵ�
	list<_tchar* > Get_NameList() { return NameList; }
	void AddNameList(_tchar* name) { NameList.push_back(name); }
	// ~�ʿ��������� Ű������ ã�� �ϴ��ڵ�

private:
	map<const _tchar*, CGameObject*>			m_mapObject;


	list<CGameObject*>  m_objPoolList;

	list<_tchar* > NameList;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);

};

END