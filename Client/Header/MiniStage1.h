#pragma once
#include "Engine_Include.h"
#include "Scene.h"

#include "BackGround.h"
#include "Loading.h"
// Button
#include "Start_Button.h"
#include "Exit_Button.h"
#include "SettingButton.h"

class CMiniStage1 :public CScene
{
private:
	explicit CMiniStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniStage1();
	
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
	CLoading*			m_pLoading = nullptr;



private: // Press Start Button ±ôºýÀÌ°Ô ÇÏ±â À§ÇÔ
	_uint				m_iCount = 0;
	_bool				m_bRender = false;

	// Test
	_bool				m_bFinish;
public:
	static CMiniStage1*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);

};

