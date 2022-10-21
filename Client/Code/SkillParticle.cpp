#include "stdafx.h"
#include "..\Header\SkillParticle.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Player.h"

CSkillParticle::CSkillParticle(LPDIRECT3DDEVICE9 pGraphicDev, BDBOX * boundingBox, int numParticles, _vec3 vPos)
	:CPSystem(pGraphicDev)
{
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Eletric_Particle_Texture", m_mapComponent, ID_STATIC);
	
	m_bdBox = *boundingBox;
	m_fSize = 0.1f;
	m_vbSize = 32;
	m_vbOffset = 0;
	m_vbBatchSize = 8;

	for (int i = 0; i < numParticles; i++)
	{
		addParticle();
	}
	
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);



}

CSkillParticle::~CSkillParticle()
{
}

_int CSkillParticle::Update_Object(const _float & fTimeDelta)
{
	if (m_bActive)
	{
		m_fFrame += 1.f*fTimeDelta;

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(::Get_GameObject(L"Layer_GameLogic",L"Player"));
		_vec3 vPos;
		
		CTransform* pPlayerTransform = static_cast<CTransform*>(pPlayer->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
		pPlayerTransform->Get_Info(INFO_POS, &vPos);
		_vec3 vAngle;
		vAngle = pPlayerTransform->Get_Angle();

		m_pTransform->Rotation(ROT_Y, vAngle.y);
		if (vAngle.y > 0)
			vPos.x += 1.f;
		else
			vPos.x -= 1.f;
		m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}

	if (m_fFrame >= 1.f)
	{
		m_bActive = false;
		m_fFrame = 0.f;
	}
	update(fTimeDelta);



	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CSkillParticle::update(_float fTimeDelta)
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

void CSkillParticle::Render_Obejct(void)
{
	if (!m_bActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	render();

	

}

void CSkillParticle::resetParticle(Attribute* attribute)
{
	//중앙으로부터 팔방으로 퍼져나가는 움직임을 구현해야함
	attribute->_isAlive = true;
	float x, y, z;
	
	x = 0.5f;
	y = 0.5f;
	z = 0.f;

	attribute->_position = { x, y, z };

	float frand;

	if (0 == rand() % 2)
	{
		attribute->_velocity.x = ((1 + rand() % 10) * 0.1f);
	}
	else
	{
		
		attribute->_velocity.x = ((1 + rand() % 10) * -0.1f);
	}

	if (0 == rand() % 2)
	{
		attribute->_velocity.y = ((1 + rand() % 10) * 0.1f);
	}
	else
	{

		attribute->_velocity.y = ((1 + rand() % 10) * -0.1f);
	}

	m_fSize = 0.1f;
	attribute->_velocity.z = 1.f;

	attribute->_color = D3DCOLOR_XRGB(255, 255, 255);
}

CSkillParticle * CSkillParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	BDBOX tempbox;
	tempbox.vMin = _vec3(0.f, 0.f, 0.f);
	tempbox.vMax = _vec3(1.f, 1.f, 3.f);
	CSkillParticle *	pInstance = new CSkillParticle(pGraphicDev, &tempbox, 64, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSkillParticle::Free(void)
{
	CPSystem::Free();
}
