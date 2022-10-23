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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 54)), E_FAIL);

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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 6)), E_FAIL);
	
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
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Anubis_Thunder_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis_laser.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/Sphinx%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_Bullet_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/SphinxBullet%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_HeadOff_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/sphinx_headoff%d.png", TEX_NORMAL, 20)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_body_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/sphinx_body.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_Activated_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/sphinx_active%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_FlyHead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/sphinx_flyhead%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_lrattack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/sphinx_lrattack%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_bodyattack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/BodyAttack/bodyattack_charge%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/SphinxDead/sphinx_dead%d.png", TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Anubis_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis_dead%d.png", TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Fatbat_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/fatbat_dead%d.png", TEX_NORMAL, 15)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Spider_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/spider_death%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obelisk_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Obelisk/Obelisk%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obelisk_Summon_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Obelisk/summon_effect%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obelisk_Wreck_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Obelisk/ObeliskWreck%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Ghul_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Ghul/Ghul%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dead_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Ghul_dig_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Ghul/Ghul_dig%d.png", TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Ghul_Attack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Ghul/Ghul_Attack%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Ghul_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Ghul/Ghul_Dead%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EarthShaker_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/EarthShaker/earthshaker%d.png", TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EarthShaker_Attack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/EarthShaker/earthshaker_attack%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EarthShaker_Dead_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/EarthShaker/earthshaker_dead%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Anubis_Thunder_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis_thunder%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Anubis_Stormball_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/stormball%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EarthShaker_Spikeone_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/EarthShaker/spikeone%d.png", TEX_NORMAL, 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EarthShaker_Spiketwo_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/EarthShaker/spiketwo%d.png", TEX_NORMAL, 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FireTrap_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Trap/FireTrap%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FirePillar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Trap/FirePillar%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FireReady_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Trap/FireReady%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Curse_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/Spell_Curse.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Soldier_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Soldier/Soldier%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Soldier_Attack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Soldier/Soldier_attack%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Soldier_Death_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Soldier/Soldier_death%d.png", TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Soldier_Bullet_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Soldier/Soldierbullet%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Harpoonguy_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Harpoonguy/harpoonguy%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Harpoonguy_Attack_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Harpoonguy/harpoonguy_attack%d.png", TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Harpoonguy_Death_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Harpoonguy/harpoonguy_death%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Harpoonguy_Bullet_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Harpoonguy/redbull%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackA_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingyattackA%d.png", TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackB_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingyattackB%d.png", TEX_NORMAL, 28)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackC_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingyattackC%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_Morph_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingy_morph%d.png", TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingy%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_spike_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingy_spike%d.png", TEX_NORMAL, 20)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_tentacle_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingy_tentacle%d.png", TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_Onekiok_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/Onekiok/Onekiok%d.png", TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_Death_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Thingy/thingy_death%d.png", TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TapeWorm_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/TapeWorm/tapeworm%d.png", TEX_NORMAL, 28)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TapeWorm_death_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/TapeWorm/tapeworm_death%d.png", TEX_NORMAL, 6)), E_FAIL);
	
	// ~Monster

	// _UI																								
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Bullet_UI/Bullet_UI%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Weapon_UI/Weapon%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpBar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/HpBar/HpBar%d.png", TEX_NORMAL, 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StatusUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Status/Status_UI1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Coin_KeyUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Status/Key_Coin_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArrowTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Arrow/Arrow%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Gun_ScreenTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/Magnum%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dash_UI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Dash_Icon1.png", TEX_NORMAL)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Inventory_UI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Inventory_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shop_UI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Shop_UI1.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Chim_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Chim_Binpole.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Skill_ScreenTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/Skill_Screen/spellhand_storm_%d.png", TEX_NORMAL, 5)), E_FAIL);

	
	// \Bin\Resource\Texture\UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Frame_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/UI_Frame1.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ChangeScene_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/Change_Scene/Change_Scene%d.png", TEX_NORMAL, 7)), E_FAIL);
	// \Bin\Resource\Texture\UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LoadingFontBackGround_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/LoadingFont_Background.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGun_ScreenTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/Shotgun/Shotgun%d.png", TEX_NORMAL, 16)), E_FAIL);																									// \Bin\Resource\Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FadeOut_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Fade_Out.png", TEX_NORMAL)), E_FAIL);																										// \Bin\Resource\Texture\Logo
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SettingsScene_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/SettingsScene.png", TEX_NORMAL)), E_FAIL);
																											//\Bin\Resource\Texture\UI	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SoundBox_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/SoundBox.png", TEX_NORMAL)), E_FAIL);
																										// Bin\Resource\Texture\UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuizBox1_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/QuizBox1.png", TEX_NORMAL)), E_FAIL);
																										// \Bin\Resource\Texture\Bullet_UI\Shotgun_Shell
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunShellUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Bullet_UI/Shotgun_Shell/Sg_Shell%d.png", TEX_NORMAL, 7)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CurrectUI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Currect/Currect%d.png", TEX_NORMAL, 6)), E_FAIL);
	// AX 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AX_ScreenTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Weapon_UI/AX/AX_%d.png", TEX_NORMAL, 10)), E_FAIL);
																										// \Bin\Resource\Texture\HpBar\DefBar
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DefBar_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/HpBar/DefBar/DefBar%d.png", TEX_NORMAL, 11)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Skill_Icon_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Skill_Icon.png", TEX_NORMAL)), E_FAIL);

																										// \Bin\Resource\Texture\UI\Shop_Thunder
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shop_Sold_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Shop_Thunder/Thunder_Sold%d.png", TEX_NORMAL, 2)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BloodScreen_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Blood_Screen1.png", TEX_NORMAL)), E_FAIL);
																												// \Bin\Resource\Texture\UI\Shop_Helmet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Helmet_Sold_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Shop_Helmet/Helmet_Sold%d.png", TEX_NORMAL, 2)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Minigame1Pic_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Shop_Chim/Chim_Icon%d.png", TEX_NORMAL, 2)), E_FAIL);
	// ~UI

	// Box
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Box/Box%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CoinTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Coin/Coin%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeNormalTexCom", CCubeNormalTex::Create(m_pGraphicDev)), E_FAIL);
	// ~Box

	// Particle

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Rain/Droplet.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Eletric_Particle_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Electric/Electric_Particle.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FireworksTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/Fireworks%d.png", TEX_NORMAL, 7)), E_FAIL);
	
	// ~particle


	// CubeMap
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeMap/Cube%d.dds", TEX_CUBE, 62)), E_FAIL);

	// ~CubeMap

	// ShotGun 																			 // \Bin\Resource\Texture\Equipment\Equip-Weapon/Shotgun_Rot.png
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip-Weapon/Equip_Weapon1.png", TEX_NORMAL)), E_FAIL);
	// Magnum 																			 // \Bin\Resource\Texture\Equipment\Equip-Weapon/Shotgun_Rot.png
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MagnumTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip-Weapon/Equip_Weapon0.png", TEX_NORMAL)), E_FAIL);

	//PetYeti
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Equip_YetiTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip_Pet/Equip_Yeti.png", TEX_NORMAL)), E_FAIL);

	// Helmet1
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Helmet1Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip_HelMet/Helmet3.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Helmet2Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Equipment/Equip_HelMet/Helmet2.png", TEX_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ThunderTexCom", CThunderTex::Create(m_pGraphicDev)), E_FAIL);
	//작업할때 create clone 추가할것


	// Obstacle Anim
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CrystralAnimTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/features/CrystralAnim_0/featureAnim_%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StreetTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/features/StreetLamp_1/featureAnim_%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TreeAnimTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/features/TreeAnim_2/features_%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WallLampTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/features/WallLamp_3/features_%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FireTexCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/features/Fire_4/Fire_%d.png", TEX_NORMAL, 16)), E_FAIL);
	// Obstacle Anim


	// Effect
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_hitbloodTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/hitblood_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_hitbloodTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/hitblood2_%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_hitbloodTexture3", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/hitblood3_%d.png", TEX_NORMAL, 8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_bullet_shell", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/bullet_shell.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_shotgun_shell", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/shotgunshell.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AttackPlayerAXTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/AX/boom_air%d.png", TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_explosion_sphinx", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/explosion_midair%d.png", TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HitHoleTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/bighole_01.png", TEX_NORMAL)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_OverKillTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Overkill/overkill_%d.png", TEX_NORMAL,18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DashingEffectTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Dashing/Dashing_%d.png", TEX_NORMAL,5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Flaretexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/flare%d.png", TEX_NORMAL, 6)), E_FAIL);
	// 

	//Shader
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderTerrain", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFilesPath/Shader_Terrain.hlsl"))), E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_fetrues_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/features/features_%d.png", TEX_NORMAL, 24)), E_FAIL);

	// NPC
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_NPCTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/ShopNPC/NPC_%d.png", TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuestNPCTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/QuestNPC/QuestNpc_%d.png", TEX_NORMAL,8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuestTalking_Frame_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/QuestNPC/QuestDialog.png", TEX_NORMAL)), E_FAIL);


	// ~ NPC
	
	// Pet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Yeti_TextureCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Pet/Yeti/yeti%d.png", TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Yeti_dance_TextureCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Pet/Yeti/yeti_dance%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Yeti_talk_TextureCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Pet/Yeti/yeti_talk.png", TEX_NORMAL, 1)), E_FAIL);
	//~pet

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KeyTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Key.png", TEX_NORMAL)), E_FAIL);
	
	// CoinSlot																						// \Bin\Resource\Texture\CoinSlot
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CoinSlotTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CoinSlot/CoinSlot%d.png", TEX_NORMAL, 2)), E_FAIL);

	m_bFinish = true;

	lstrcpy(m_szLoading, L"Press Start Button");

	return 0;
}

_uint CLoading::Loading_ForBoss(void)
{
	return 0;
}

_uint CLoading::Loading_ForPlayer_UI(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 54)), E_FAIL);
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
	


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGun_ScreenTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Output/Shotgun/Shotgun%d.png", TEX_NORMAL, 16)), E_FAIL);

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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Tile/textures_%d.png", TEX_NORMAL, 54)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriColCom", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcColCom", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/Cube%d.dds", TEX_CUBE, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeMap/Cube%d.dds", TEX_CUBE, 62)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereTexCom", CSphereTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeMapTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeMap/Cube1.dds", TEX_CUBE)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HpPotionTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/HpPotion/HpPotion_%d.png", TEX_NORMAL, 4)), E_FAIL);

	//monstertool
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Monster_Example_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Example/Example_%d.png", TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Fatbat/fatbat_fly%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture3", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Spider/Spider%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CharacterInfoCom", CCharacterInfo::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DynamicTransformCom", CDynamic_Transform::Create()), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ColliderCom", CCollider::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Dead_AnimationCom", CAnimation::Create(m_pGraphicDev, 0, 0, 0.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sphinx_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Sphinx/Sphinx%d.png", TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Obelisk_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Obelisk/Obelisk%d.png", TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Ghul_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Ghul/Ghul%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EarthShaker_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/EarthShaker/earthshaker%d.png", TEX_NORMAL, 10)), E_FAIL);

	//~

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_fetrues_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/features/features_%d.png", TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeNormalTexCom", CCubeNormalTex::Create(m_pGraphicDev)), E_FAIL);
	// ㅇㄴㅁㄻㄴ
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderTerrain", CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFilesPath/Shader_Terrain.hlsl"))), E_FAIL);


	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Monster/Anubis/anubis%d.png", TEX_NORMAL, 8)), E_FAIL);
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

	case LOADING_BOSS:
		// iFlag = pLoading->Loading_ForBoss();
		break;

	case LOADING_TEST:

		break;

	case LOADING_MINISTAGE3:
		iFlag = pLoading->Loading_ForStage();
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
