#pragma once
#include "Scene.h"


class CBossStage :public CScene
{
private:
	explicit CBossStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossStage();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

	void	Copy_Scene_Data(CScene* pStage1Scene);


private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Icon(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_CubeCollsion(const _tchar * pLayerTag);
	HRESULT			Ready_Layer_Room(const _tchar* pLayerTag);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);

	_float				m_fFrame = 0.f; // WinZero  µµÂø½Ã°£

	_bool		m_bStopBGM = false;

public:
	static CBossStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free(void);


};

