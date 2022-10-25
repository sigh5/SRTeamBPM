#pragma once
#include "Engine_Include.h"
#include "Scene.h"


class CEndingScene : public CScene
{
public:
	explicit CEndingScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEndingScene();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag) { return S_OK; }

	HRESULT			Ready_Proto(void);

private:
	_float			m_fCount = 1200.f;
	
public:
	static CEndingScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};

