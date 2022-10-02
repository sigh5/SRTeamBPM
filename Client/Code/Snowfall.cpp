#include "stdafx.h"
#include "..\Header\Snowfall.h"

#include "Export_Function.h"
#include "AbstractFactory.h"


CSnowfall::CSnowfall(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX * boundingBox, int numParticles)
	:CPSystem(pGraphicDev)
{
	m_bdBox = *boundingBox;
	m_fSize = 0.6f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
	{
		addParticle();
	}

	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_RainTexture", m_mapComponent, ID_STATIC);

}


void CSnowfall::resetParticle(Attribute * attribute)
{
	attribute->_isAlive = true;
	float x, y, z;

	x = (float)(rand() % 130);
	y = 50.f;
	z = (float)(rand() % 130);

	//GetRandomVector(&attribute->_position, &m_bdBox.vMin, &m_bdBox.vMax);
	
	attribute->_position = { x, y, z };

	//attribute->_position.y = m_bdBox.vMax.y;

	//attribute->_velocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->_velocity.x = rand() % 10 * 0.1f * -1.0f;
	//x 이동값

	attribute->_velocity.y = rand() % 10 * 0.1f * -30.0f + 1;
	//내리는 속도

	attribute->_velocity.z = 0.f;

	attribute->_color = D3DCOLOR_XRGB(170, 0, 0);
	//컬러 색
	//++m_ResetCount;
}

CSnowfall::~CSnowfall()
{
}

_int CSnowfall::Update_Object(const _float& fTimeDelta)
{
	

	update(fTimeDelta);



	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}


void CSnowfall::update(_float fTimeDelta)
{
	for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
	{
		
		i->_position += i->_velocity * fTimeDelta;

		if (m_bdBox.vMax.x < i->_position.x || m_bdBox.vMin.x > i->_position.x)
		{
			resetParticle(&(*i));
		}
		else if(m_bdBox.vMax.y < i->_position.y || m_bdBox.vMin.y > i->_position.y)
		{
			resetParticle(&(*i));
		}
		else if (m_bdBox.vMax.z < i->_position.z || m_bdBox.vMin.z > i->_position.z)
		{
			resetParticle(&(*i));
		}

	}

	Add_RenderGroup(RENDER_ALPHA, this);
}
void	CSnowfall::Render_Obejct(void)
{
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	render();

}

CSnowfall * CSnowfall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	BDBOX tempbox;
	tempbox.vMin = _vec3(0.f, 0.f, 0.f);
	tempbox.vMax = _vec3(130.f, 50.f, 130.f);
	CSnowfall *	pInstance = new CSnowfall(pGraphicDev, &tempbox, 2048);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSnowfall::Free(void)
{
	CPSystem::Free();

}
