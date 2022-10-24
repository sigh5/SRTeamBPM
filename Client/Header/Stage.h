#pragma once

#include "Engine_Include.h"
#include "Scene.h"


#include "Bullet.h"  // Player's
#include "MonsterBullet.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "Start_Button.h"
#include "Bullet_UI.h"
#include "Weapon_UI.h"


class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

	void			Set_SphinxBGM(_bool _bSphinxBGM) { m_bSpinxBGM = _bSphinxBGM; }
	void			Set_SphinxHeadBGM(_bool _bSphinxHeadBGM) { m_bSphinxHeadBGM = _bSphinxHeadBGM; }
	void			Set_FinalBGM(_bool _bFinalBGM) { m_bFinalBGM = _bFinalBGM; }
private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Icon(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_CubeCollsion(const _tchar * pLayerTag);
	HRESULT			Ready_Layer_Room(const _tchar* pLayerTag);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);

	void			TeleportCubeUpdate(const _float& fTimeDelta);
	void			Set_Player_StartCubePos();

private:
	list<_tchar*>		m_szEffectName;
	_uint				m_iCount = 0;
	

	_float				m_fFrame = 0.f; // WinZero  µµÂø½Ã°£

	_bool		m_bStopBGM = false;
	_bool		m_bOnce = false;

	_bool		m_bSpinxBGM = false;
	_bool		m_bSphinxHeadBGM = false;
	_bool		m_bFinalBGM = false;

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);




private:
	virtual void	Free(void);


	


};

