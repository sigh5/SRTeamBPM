#pragma once

#include "Engine_Include.h"
#include "Scene.h"


#include "TestPlayer.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "Monster.h" //테스트용 몬스터
#include "HpPotion.h"


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

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT			Ready_Monster(void);
	HRESULT			Ready_HpPotion(void);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);

	//CObjectPool<CMonster>* m_MonsterPool;

private:
	list<CMonster*>		m_MonsterList; //몬스터 리스트
	list<CHpPotion*>	m_PotionList;

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};

