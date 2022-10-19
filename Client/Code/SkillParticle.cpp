#include "stdafx.h"
#include "..\Header\SkillParticle.h"

#include "Export_Function.h"

CSkillParticle::CSkillParticle(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX * boundingBox, int numParticles, _vec3 vPos)
	:CPSystem(pGraphicDev)
{
}

CSkillParticle::~CSkillParticle()
{
}

_int CSkillParticle::Update_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSkillParticle::update(_float fTimeDelta)
{
}

void CSkillParticle::Render_Obejct(void)
{
}

CSkillParticle * CSkillParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	return nullptr;
}

void CSkillParticle::Free(void)
{
}
