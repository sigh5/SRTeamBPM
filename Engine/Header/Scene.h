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

public:
	virtual void	Free(void);
};

END