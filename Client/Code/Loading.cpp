#include "stdafx.h"
#include "..\Header\Loading.h"

#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
	, m_eID(LOADING_END)	
{
	ZeroMemory(m_szLoading, sizeof(m_szLoading));
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpPotionTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/HpPotion/HpPotion_%d.png", TEX_NORMAL, 4)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 47)), E_FAIL);
	
	// Collider
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);
	// ~Collider

	//Cube, Sphere
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereTexCom", CSphereTex::Create(m_pGraphicDev)), E_FAIL);
	//~Cube, Sphere

	// Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Fatbat_BulletTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/FatbatBullet%d.png", TEX_NORMAL, 14)), E_FAIL);
	// ~Bullet

	// SkyBox
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);

	// ~SkyBox

	// Player
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DynamicTransformCom", CDynamic_Transform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CharacterInfoCom", CCharacterInfo::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	// ~Player

	// Monster
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/fatbat_fly%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture3", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/Spider%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Spider_Attack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/Spider_Attack%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Anubis_Attack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis_Attack%d.png", TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Anubis_Thunder_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis_laser.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/Sphinx%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_Bullet_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/SphinxBullet%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Anubis_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis_dead%d.png", TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Fatbat_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/fatbat_dead%d.png", TEX_NORMAL, 15)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Spider_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/spider_death%d.png", TEX_NORMAL, 14)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dead_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	
	// ~Monster

	// _UI

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Bullet_UI/Bullet_UI%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Weapon_UI/Weapon%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpBar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/HpBar/HpBar%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StatusUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Status/Status_UI1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Coin_KeyUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Status/Key_Coin_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArrowTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Arrow/Arrow%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Gun_ScreenTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/Magnum%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dash_UI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Dash_Icon.png", TEX_NORMAL)), E_FAIL);
	
	// ~UI

	// Box
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Box/Box%d.png", TEX_NORMAL, 2)), E_FAIL);
	// ~Box

	// Particle

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Rain/Droplet.png", TEX_NORMAL, 1)), E_FAIL);

	// ~particle


	// CubeMap
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeMap/Cube%d.dds", TEX_CUBE, 19)), E_FAIL);

	// ~CubeMap

	// ShotGun 																			 // \Bin\Resource\Texture\Equipment\Equip-Weapon/Shotgun_Rot.png
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip-Weapon/Equip_Weapon1.png", TEX_NORMAL, 1)), E_FAIL);
	// Magnum 																			 // \Bin\Resource\Texture\Equipment\Equip-Weapon/Shotgun_Rot.png
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MagnumTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip-Weapon/Equip_Weapon0.png", TEX_NORMAL, 1)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ThunderTexCom", CThunderTex::Create(m_pGraphicDev)), E_FAIL);
	//작업할때 create clone 추가할것

	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!");

	return 0;
}

_uint CLoading::Loading_ForColliderStage(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 47)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture2", 
	CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Select/Player%d.jpg", TEX_NORMAL, 6)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriColCom", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcColCom", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	// Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster1.png", TEX_NORMAL)), E_FAIL);
	// ~Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpPotionTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/HpPotion/HpPotion_%d.png", TEX_NORMAL, 4)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	// Collider
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);
	// ~Collider
	
	// UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UItexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/hpbar1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArrowTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Arrow/Arrow%d.png", TEX_NORMAL,2)), E_FAIL);
	// ~UI
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DynamicTransformCom", CDynamic_Transform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CharacterInfoCom", CCharacterInfo::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	// 
	// ~UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Bullet_UI/Bullet_UI%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Weapon_UI/Weapon%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/fatbat_fly%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpBar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/HpBar/HpBar%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis%d.png", TEX_NORMAL, 8)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeMap/Cube%d.dds", TEX_CUBE, 19)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_hitbloodTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/hitblood_%d.png", TEX_NORMAL, 8)), E_FAIL);
	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!");

	return 0;
}

_uint CLoading::Loading_ForBoss(void)
{
	return 0;
}

_uint CLoading::Loading_ForTest(void)
{

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 47)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriColCom", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcColCom", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	// Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster1.png", TEX_NORMAL)), E_FAIL);
	// ~Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/fatbat_fly%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture3", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/Spider%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpPotionTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/HpPotion/HpPotion_%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Bullet_UI/Bullet_UI%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Weapon_UI/Weapon%d.png", TEX_NORMAL, 2)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Rain/Droplet.png", TEX_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	// Collider
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);
	// ~Collider

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CharacterInfoCom", CCharacterInfo::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	// UI

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UItexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/hpbar1.png", TEX_NORMAL)), E_FAIL);

	// ~UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DynamicTransformCom", CDynamic_Transform::Create()), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_hitbloodTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/hitblood_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Spider_Attack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/Spider_Attack%d.png", TEX_NORMAL, 14)), E_FAIL);
	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!");

	return 0;
}

_uint CLoading::Loading_ForPlayer_UI(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 47)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriColCom", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcColCom", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	// Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster1.png", TEX_NORMAL)), E_FAIL);
	// ~Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/fatbat_fly%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture3", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/Spider%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	
																									// Bin\Resource\Texture\Gun_Output
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Gun_ScreenTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/Magnum%d.png", TEX_NORMAL, 5)), E_FAIL);


	// Item Texture

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpPotionTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/HpPotion/HpPotion_%d.png", TEX_NORMAL, 4)), E_FAIL);
																									// Bin\Resource\Texture\Item\Coin
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CoinTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Coin/Coin%d.png", TEX_NORMAL, 6)), E_FAIL);
																									//\Bin\Resource\Texture\Item
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KeyTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Key.png", TEX_NORMAL)), E_FAIL);

	// Box																						// Bin\Resource\Texture\Item\Box
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Box/Box%d.png", TEX_NORMAL, 2)), E_FAIL);

	// ShotGun 																			 // \Bin\Resource\Texture\Equipment\Equip-Weapon/Shotgun_Rot.png
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip-Weapon/Equip_Weapon1.png", TEX_NORMAL, 1)), E_FAIL);
	// Magnum 																			 // \Bin\Resource\Texture\Equipment\Equip-Weapon/Shotgun_Rot.png
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MagnumTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip-Weapon/Equip_Weapon0.png", TEX_NORMAL, 1)), E_FAIL);

	// ~Item Texture


	// UI Texture
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Bullet_UI/Bullet_UI%d.png", TEX_NORMAL, 9)), E_FAIL);	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Weapon_UI/Weapon%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpBar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/HpBar/HpBar%d.png", TEX_NORMAL, 5)), E_FAIL);																									
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StatusUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Status/Status_UI1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Coin_KeyUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Status/Key_Coin_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dash_UI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Dash_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Inventory_UI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Inventory_UI.png", TEX_NORMAL)), E_FAIL);
																											// \Bin\Resource\Texture\UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Frame_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/UI_Frame.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ChangeScene_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/Change_Scene/Change_Scene%d.png", TEX_NORMAL, 7)), E_FAIL);
																												// \Bin\Resource\Texture\UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LoadingFontBackGround_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/LoadingFont_Background.png", TEX_NORMAL)), E_FAIL);
	// ~UI Texture																						

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CharacterInfoCom", CCharacterInfo::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DynamicTransformCom", CDynamic_Transform::Create()), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);
	
	m_bFinish = true;

	lstrcpy(m_szLoading, L"Press Start Button");

	return 0;
}

_uint CLoading::Loading_For_ToolScene()
{

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 47)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriColCom", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcColCom", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/Cube%d.dds", TEX_CUBE, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeMap/Cube%d.dds", TEX_CUBE, 19)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereTexCom", CSphereTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeMapTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeMap/Cube1.dds", TEX_CUBE)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpPotionTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/HpPotion/HpPotion_%d.png", TEX_NORMAL, 4)), E_FAIL);
	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!");

	return 0;
}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	_uint iFlag = 0;

	EnterCriticalSection(&(pLoading->Get_Crt()));

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_COLLIDER:
		iFlag = pLoading->Loading_ForColliderStage();
		break;

	case LOADING_BOSS:
		// iFlag = pLoading->Loading_ForBoss();
		break;
		
	case LOADING_TEST:
		iFlag = pLoading->Loading_ForTest();
		break;

	case LOADING_PLAYER_UI:
		iFlag = pLoading->Loading_ForPlayer_UI();
		break;

	case LOADING_TOOL:
		iFlag = pLoading->Loading_For_ToolScene();
		break;
	}
	
	LeaveCriticalSection(&(pLoading->Get_Crt()));

	return iFlag;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		MSG_BOX("Loading Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
