#pragma once

#include "Engine_Include.h"
#include "Scene.h"

#include "TestPlayer.h"
#include "Bullet.h"  // Player's
#include "MonsterBullet.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "Start_Button.h"
#include "Bullet_UI.h"
#include "Weapon_UI.h"

class CStage_BIH : public Engine::CScene
{
private:
	explicit CStage_BIH(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage_BIH();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

	HRESULT			Push_Bullet(CBullet* pBullet);
	HRESULT			Push_MonBullet(CMonsterBullet* pBullet);
private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);

private:
	list<_tchar*>		m_szEffectName;
	_uint				m_iCount = 0;
	list<CBullet*>		m_BulletList;
	list<CMonsterBullet*> m_MonBulletList;

	CStart_Button*		m_pStartButton = nullptr;

public:
	static CStage_BIH*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};
