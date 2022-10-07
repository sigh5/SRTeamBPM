#include "stdafx.h"
#include "..\Header\Obelisk.h"

#include "Export_Function.h"
#include "AbstractFactory.h"

CObelisk::CObelisk(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CObelisk::~CObelisk()
{
}

HRESULT CObelisk::Ready_Object(int Posx, int Posy)
{
	NULL_CHECK_RETURN(Add_Component(), -1);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Obelisk_Texture", m_mapComponent, ID_STATIC);

	m_pInfoCom->Ready_CharacterInfo(5, 10, 5.f);
	m_iPreHp = (m_pInfoCom->Get_InfoRef()._iHp);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		Set_Pos(_vec3{ Posx, 0.f, Posy });
	}

	return S_OK;
}

_int CObelisk::Update_Object(const _float & fTimeDelta)
{


	return 0;
}

void CObelisk::LateUpdate_Object(void)
{
}

void CObelisk::Render_Obejct(void)
{
}

void CObelisk::Collision_Event()
{
}

bool CObelisk::Dead_Judge(const _float & fTimeDelta)
{
	return false;
}

void CObelisk::AttackJudge(const _float & fTimeDelta)
{
}

void CObelisk::Attack(const _float & fTimeDelta)
{
}

void CObelisk::NoHit_Loop(const _float & fTimeDelta)
{
}

void CObelisk::Hit_Loop(const _float & fTimeDelta)
{
}

CObelisk * CObelisk::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	return nullptr;
}

void CObelisk::Free(void)
{
}
