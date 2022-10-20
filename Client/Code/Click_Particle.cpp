#include "stdafx.h"
#include "..\Header\Click_Particle.h"

#include "Export_Function.h"
#include "AbstractFactory.h"

CClick_Particle::CClick_Particle(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX * boundingBox, _uint numParticles, _vec3 vPos)
	: CPSystem(pGraphicDev)
{
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Eletric_Particle_Texture", m_mapComponent, ID_STATIC);
	
	m_bdBox = *boundingBox;
	m_fSize = 0.1f;
	m_vbSize = 32;
	m_vbOffset = 0;
	m_vbBatchSize = 8;

	for (_uint i = 0; i < numParticles; i++)
	{
		addParticle();
	}
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	
}

CClick_Particle::~CClick_Particle()
{
}

void CClick_Particle::resetParticle(Attribute * attribute)
{
	attribute->_isAlive = true;
	attribute->_position = m_origin;
	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.f, 1.f, 1.f);

	GetRandomVector(&attribute->_velocity, &min, &max);

	D3DXVec3Normalize(&attribute->_velocity, &attribute->_velocity);

	attribute->_velocity *= 50.f;

	attribute->_color = D3DXCOLOR(GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), 1.f);

	attribute->_age = 0.f;
	attribute->_liftTime = 2.f;

}

_int CClick_Particle::Update_Object(const _float & fTimeDelta)
{
	update(fTimeDelta);

	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CClick_Particle::update(_float fTimeDelta)
{
	for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
	{
		if (i->_isAlive)
		{
			i->_position += i->_velocity * fTimeDelta;

			i->_age += fTimeDelta;

			if (i->_age > i->_liftTime)
			{
				i->_isAlive = false;
			}
		}
	}

	Add_RenderGroup(RENDER_ALPHA, this);
}

void CClick_Particle::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	render();
}

void CClick_Particle::preRender()
{
	CPSystem::preRender();

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CClick_Particle::postRender()
{
	CPSystem::postRender();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CClick_Particle * CClick_Particle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	BDBOX tempbox;
	tempbox.vMin = _vec3(0.f, 0.f, 0.f);
	tempbox.vMax = _vec3(1.f, 1.f, 3.f);
	CClick_Particle *	pInstance = new CClick_Particle(pGraphicDev, &tempbox, 64, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CClick_Particle::Free(void)
{
	CPSystem::Free();
}
