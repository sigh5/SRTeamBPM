#pragma once

#include "Engine_Include.h"
#include "Stage.h"
#include "Scene.h"



class CTestPlayer;
class CTerrain;
class CDynamicCamera;
class CMonster;

class CColliderStage :public CScene
{
private:
	explicit CColliderStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CColliderStage();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;


private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);

private:
	list<_tchar*>		m_szEffectName;
	_uint				m_iCount = 0;
	_float				m_fTimeDelta = 0.f;


public:
	static CColliderStage*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);


};

