#include "stdafx.h"
#include "..\Header\RealSnowFall.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

CRealSnowFall::CRealSnowFall(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX * boundingBox, int numParticles)
	:CPSystem(pGraphicDev)
{
	m_bdBox = *boundingBox;
	m_fSize = 0.3f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
	{
		addParticle();
	}

	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_SnowTexture", m_mapComponent, ID_STATIC);

	m_pTransform->Set_Pos(480.f, 0.f, 480.f);
}

void CRealSnowFall::resetParticle(Attribute * attribute)
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
	attribute->_velocity.x = rand() % 40 * -1.f * 0.1f;
	//x 이동값

	attribute->_velocity.y = GetRandomFloat(0.0f, 1.0f) * -10.f;

	//내리는 속도

	attribute->_velocity.z = 0.f;

	attribute->_color = D3DCOLOR_XRGB(255, 255, 255);
	//컬러 색
	//++m_ResetCount;
}

CRealSnowFall::~CRealSnowFall()
{

}

_int CRealSnowFall::Update_Object(const _float & fTimeDelta)
{
	update(fTimeDelta);



	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CRealSnowFall::update(_float fTimeDelta)
{
	for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
	{

		i->_position += i->_velocity * fTimeDelta;

		if (m_bdBox.vMax.x < i->_position.x || m_bdBox.vMin.x > i->_position.x)
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
		}

	}

	Add_RenderGroup(RENDER_ALPHA, this);
}

void CRealSnowFall::Render_Obejct(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	render();
}

CRealSnowFall * CRealSnowFall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	BDBOX tempbox;
	tempbox.vMin = _vec3(0.f, 0.f, 0.f);
	tempbox.vMax = _vec3(130.f, 50.f, 130.f);
	CRealSnowFall *	pInstance = new CRealSnowFall(pGraphicDev, &tempbox, 2048);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRealSnowFall::Free(void)
{
	CPSystem::Free();
}
