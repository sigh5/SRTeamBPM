#pragma once

#include "Base.h"
#include "Scene.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CManagement :	public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CComponent*				Get_Component(const _tchar* pLayerTag, 
		const _tchar* pObjTag, 
		const _tchar* pComponentTag,
		COMPONENTID eID);

	HRESULT			Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pInstance);
	
	

public:
	HRESULT		Set_Scene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	void		LateUpdate_Scene(void);
	void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	CGameObject*			Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	CScene*		Get_Scene() { return m_pScene; }

	HRESULT		Change_Scene(CScene* pScene, CScene* pChangeScene);

	HRESULT		Load_SaveScene(CScene* pScene); // CScene* pScene : 현재 씬임을 확인

	CScene*		Get_SaveScene() { return m_pSaveScene; }
	void		Set_SaveScene(CScene* pScene) { m_pSaveScene = pScene; }


	HRESULT		Current_Scene(CScene* pScene);

private:
	CScene*		m_pScene;

	CScene*		m_pSaveScene;

public:
	virtual void Free(void);
};
END
