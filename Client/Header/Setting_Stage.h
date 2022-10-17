#pragma once
#include "Engine_Include.h"
#include "Scene.h"
// Button
#include "BackButton.h"
#include "BGMDownBtn.h"
#include "BGMUpBtn.h"

#include "Logo.h"

class CSetting_Stage : public CScene
{
private:
	explicit CSetting_Stage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSetting_Stage();

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

private: // Button	
	CBackButton*		m_pBackButton = nullptr;
	CBGMDownBtn*		m_pBGMdnButton = nullptr;
	CBGMUpBtn*			m_pBGMupButton = nullptr;

	_bool					m_bCheck = false;

public:
	static CSetting_Stage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);

};

