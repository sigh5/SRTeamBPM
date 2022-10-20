#include "stdafx.h"
#include "..\Header\FireWorks.h"

#include "Export_Function.h"
#include "AbstractFactory.h"

CFireWorks::CFireWorks(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX * boundingBox, int numParticles, _vec3 vPos)
	:CPSystem(pGraphicDev)
{
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_FireworksTexture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	m_pAnimationCom->Ready_Animation(6, 0, 0.4f);
	m_bdBox = *boundingBox;
	m_fSize = 0.8f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 1024;

	for (int i = 0; i < numParticles; i++)
	{
		addParticle();
	}
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	
}

void CFireWorks::resetParticle(Attribute * attribute)
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

CFireWorks::~CFireWorks()
{
}

_int CFireWorks::Update_Object(const _float & fTimeDelta)
{
	update(fTimeDelta);

	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CFireWorks::update(_float fTimeDelta)
{
	if(m_pAnimationCom->m_iMaxMotion != m_pAnimationCom->m_iMotion)
	m_pAnimationCom->Move_Animation(fTimeDelta);
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
		/*if (m_bdBox.vMax.x < i->_position.x || m_bdBox.vMin.x > i->_position.x)
		{
			resetParticle(&(*i));
		}
		else if (m_bdBox.vMax.y < i->_position.y || m_bdBox.vMin.y > i->_position.y)
		{
			resetParticle(&(*i));
		}
		else if (m_bdBox.vMax.z < i->_position.z || m_bdBox.vMin.z > i->_position.z)
		{
			resetParticle(&(*i));
		}*/

	}

	Add_RenderGroup(RENDER_ALPHA, this);
}

void CFireWorks::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
	render();
}

void CFireWorks::preRender()
{
	CPSystem::preRender();

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CFireWorks::postRender()
{
	CPSystem::postRender();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CFireWorks * CFireWorks::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	BDBOX tempbox;
	tempbox.vMin = _vec3(0.f, 0.f, 0.f);
	tempbox.vMax = _vec3(150.f, 150.f, 150.f);
	CFireWorks *	pInstance = new CFireWorks(pGraphicDev, &tempbox, 2048, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFireWorks::Free(void)
{
	CPSystem::Free();
}
