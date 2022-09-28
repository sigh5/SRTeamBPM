#pragma once

#include "Engine_Include.h"
#include "Scene.h"

#include "TestPlayer.h"
#include "Bullet.h"  // Player's
#include "MonsterBullet.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
 //테스트용 몬스터

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

	HRESULT			Push_Bullet(CBullet* pBullet);
	HRESULT			Push_MonBullet(CMonsterBullet* pBullet);
private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);


	// Effect : multimap -> map issue
private:
	list<_tchar*>		m_szEffectName;
	_uint				m_iCount = 0;
	list<CBullet*>		m_BulletList;
	list<CMonsterBullet*> m_MonBulletList;

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};

