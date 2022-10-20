#pragma once
#include "Engine_Include.h"
#include "Scene.h"

#include "Loading.h"

class CChange_Stage : public Engine::CScene
{
public:
	explicit CChange_Stage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChange_Stage();

public:
	HRESULT Ready_Scene(_uint iStageNum = 0) ;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag) { return S_OK; }

	HRESULT			Ready_Proto(void);

	HRESULT			Ready_MiniStage3();

private:
	_uint			m_iLoadingCount = 0;
	
	_uint			m_iStageIndex = 0;

public:
	static CChange_Stage*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iStageNum = 0);

private:
	virtual void	Free(void);
};

