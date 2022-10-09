#pragma once

#include "Base.h"
#include "Engine_Include.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene :	public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent*				Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	map<const _tchar*, CLayer*> Get_MapLayer() { return m_mapLayer; }

public:
	virtual		HRESULT		Ready_Scene(void);
	virtual		_int Update_Scene(const _float& fTimeDelta);
	virtual		void LateUpdate_Scene(void);
	virtual		void Render_Scene(void) {}

	CGameObject* Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag);

	virtual SCENE_TYPE		Get_SceneType(void) { return m_SceneType; }

public:
	CLayer* GetLayer(const _tchar * pLayerTag)
	{
		auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

		if (iter == m_mapLayer.end())
			return nullptr;

		return iter->second;
	}
	
	void Add_Layer(CLayer* pLayer, _tchar* LayerName)
	{
		m_mapLayer.insert({ LayerName,pLayer });
	}

	void Add_Layer2(CLayer* pLayer, const _tchar* LayerName)
	{
		m_mapLayer.insert({ LayerName,pLayer });
	}

	

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const _tchar*, CLayer*>		m_mapLayer;

	// 현재 어떤 씬인지 알기 위한 ENUM 타입 변수 	
	SCENE_TYPE			m_SceneType;

	// 룸이동을 도와주는 게임오브젝트를 묶는 리스트
	
public:
	virtual void	Free(void);
};

END