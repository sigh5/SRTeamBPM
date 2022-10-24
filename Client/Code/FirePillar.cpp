#include "stdafx.h"
#include "..\Header\FirePillar.h"

#include "AbstractFactory.h"
#include "Export_Function.h"
#include "MyCamera.h"
#include "Player.h"

CFirePillar::CFirePillar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CFirePillar::~CFirePillar()
{
}

HRESULT CFirePillar::Ready_Object(float Posx, float Posy, float Posz)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (Posx == 0 && Posy == 0 && Posz == 0) {}
	else
	{
		m_pTransform->Set_Pos((_float)Posx, Posy, (_float)Posz);
	}
	m_pTransform->Set_Scale(&_vec3(3.f, 1.f, 3.f));
	m_pAnimationCom->Ready_Animation(8, 0, 0.1f);
	m_pReadyAnimationCom->Ready_Animation(2, 0, 1.f);
	m_pAnimationCom->m_iMotion = rand() % 9;
	m_fLifetime = 3.f;
	m_bHitPlayer = false;

	m_pTransform->Update_Component(1.f);

	return S_OK;
}

_int CFirePillar::Update_Object(const _float & fTimeDelta)
{
	m_fLifetimeCount += fTimeDelta;
	if (m_fLifetime < m_fLifetimeCount)
	{
		m_bDead = true;
		::StopSound(SOUND_TRAP);
		return 1;
	}
	switch (m_iState)
	{
	case 0:
		m_pReadyAnimationCom->Move_Animation(fTimeDelta);
		break;
	case 1:
		m_pAnimationCom->Move_Animation(fTimeDelta);
		break;
	}
	if (0 == m_iState)
	{
		if (false == m_bReadySound)
		{
			::StopSound(SOUND_TRAP);
			::PlaySoundW(L"Flamethrower_shoot.wav", SOUND_TRAP, 0.4f);
			m_bReadySound = true;
		}
	}
	if (2 == m_pReadyAnimationCom->m_iMotion)
	{
		m_iState = 1;
	}
	
	if (1 == m_iState)
	{
		if (false == m_bFlameSound)
		{
			::StopSound(SOUND_TRAP);
			::PlaySoundW(L"Flamethrowerloop.wav", SOUND_TRAP, 0.4f);
			m_bFlameSound = true;
		}
	}
	_vec3 vThunderPos = m_pTransform->m_vInfo[INFO_POS];
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vPlayerPos = pPlayerTransformCom->m_vInfo[INFO_POS];
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	float fDistance = sqrtf((powf(vThunderPos.x - vPlayerPos.x, 2) + powf(vThunderPos.y - vPlayerPos.y, 2) + powf(vThunderPos.z - vPlayerPos.z, 2)));

	if (1 == m_iState && fDistance < 1.8f && false == m_bHitPlayer)
	{
		pPlayerInfo->Receive_Damage(10);
		pPlayer->Set_DefenseToHp(true);
		m_bHitPlayer = true;
	}

	Render_Obejct();

	m_pTransform->Update_Component(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	LateUpdate_Object();
	return 0;
}

void CFirePillar::LateUpdate_Object(void)
{

	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);

	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pTransform->m_vScale.x, m_pTransform->m_vScale.y, m_pTransform->m_vScale.z);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransform->Set_WorldMatrix(&(matWorld));

	Engine::CGameObject::LateUpdate_Object();
}

void CFirePillar::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	switch (m_iState)
	{
	case 0:
		m_pReadyTextureCom->Set_Texture(m_pReadyAnimationCom->m_iMotion);

		m_pReadyBufferCom->Render_Buffer();
		break;
	case 1:
		m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

		m_pBufferCom->Render_Buffer();
		break;
	}
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CFirePillar::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_FirePillar_Texture", m_mapComponent, ID_STATIC);
	m_pReadyTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_FireReady_Texture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CThunderTex>::Clone_Proto_Component(L"Proto_ThunderTexCom", m_mapComponent, ID_STATIC);
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);
	m_pReadyBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pReadyAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_Dead_AnimationCom", m_mapComponent, ID_STATIC);
	return S_OK;
}

CFirePillar * CFirePillar::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Posx, _float Posy, _float Posz)
{
	CFirePillar*	pInstance = new CFirePillar(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, Posz)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFirePillar::Free(void)
{
	CGameObject::Free();
}
