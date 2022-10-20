#pragma once
#include "Scene.h"



class CMiniGame3 :public CScene
{
private:
	explicit CMiniGame3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniGame3();

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
	_bool				m_bRender = false;

public:
	static CMiniGame3*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);



};

