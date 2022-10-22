#include "stdafx.h"
#include "..\Header\EarthSpike.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"

CEarthSpike::CEarthSpike(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEarthSpike::~CEarthSpike()
{
}

HRESULT CEarthSpike::Ready_Object(float Posx, float Posy, bool spiketype, float Wait)
{
	m_pTextureComOne = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_EarthShaker_Spikeone_Texture", m_mapComponent, ID_STATIC);
	m_pTextureComTwo = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_EarthShaker_Spiketwo_Texture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransform = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);
	
	m_pAnimationCom->Ready_Animation(10, 0, 0.17f);

	m_pTransform->Set_Scale(&_vec3(2.f, 4.f, 2.f));

	m_pTransform->Set_Pos(Posx, m_pTransform->m_vScale.y * 0.5f, Posy);

	m_bSpikeType = spiketype;

	m_fWaitTime = Wait;
	m_fSoundWait = rand() % 300 * 0.01f;
	m_fSoundWait2 = rand() % 300 * 0.01f;
	m_fSoundWait3 = rand() % 300 * 0.01f;

	m_pTransform->Update_Component(1.f);
	return S_OK;
}

_int CEarthSpike::Update_Object(const _float & fTimeDelta)
{
	_vec3 vThunderPos = m_pTransform->m_vInfo[INFO_POS];
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CCharacterInfo* pPlayerInfo = static_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));
	_vec3 vPlayerPos = pPlayerTransformCom->m_vInfo[INFO_POS];
	if (m_pAnimationCom->m_iMotion < 4 || m_pAnimationCom->m_iMotion > 7)
	{
		m_fToPlayerDistance = 100;
	}
	else
	{
		m_fToPlayerDistance = sqrtf((powf(vThunderPos.x - vPlayerPos.x, 2) + powf(vThunderPos.y - vPlayerPos.y, 2) + powf(vThunderPos.z - vPlayerPos.z, 2)));
	}
	m_pTransform->Set_Y(m_pTransform->m_vScale.y * 0.5f);
	m_fWaitTimeCounter += fTimeDelta;
	if (m_fWaitTime > m_fWaitTimeCounter)
	{
		return 0;
	}
	if (m_pAnimationCom->m_iMaxMotion == m_pAnimationCom->m_iMotion)
	{
		m_bDead = true;
		return 1;
	}
	m_pAnimationCom->Move_Animation(fTimeDelta);




	m_fSoundWait -= fTimeDelta;

	/*if (5 < m_pAnimationCom->m_iMotion &&m_fSoundWait < 0.f && fDistance < 19)
	{
		fVolume = fDistance * 0.05f;
		::PlaySoundW(L"CrashRock.wav", SOUND_CRUSHROCK, 1.f - fVolume);
	}
	if (6 < m_pAnimationCom->m_iMotion &&m_fSoundWait2 < 0.f && fDistance < 19)
	{
		fVolume = fDistance * 0.05f;
		::PlaySoundW(L"CrashRock2.wav", SOUND_CRUSHROCK2, 1.f - fVolume);
	}
	if (7 < m_pAnimationCom->m_iMotion && m_fSoundWait3 < 0.f && fDistance < 19)
	{
		fVolume = fDistance * 0.05f;
		::PlaySoundW(L"CrashRock3.wav", SOUND_CRUSHROCK3, 1.f - fVolume);
	}*/
	//수정필요
	Render_Obejct();

	m_pTransform->Update_Component(1.f);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	LateUpdate_Object();
	return 0;
}

void CEarthSpike::LateUpdate_Object(void)
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

	// 빌보드 에러 해결
	Engine::CGameObject::LateUpdate_Object();
}

void CEarthSpike::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (m_bSpikeType)
		m_pTextureComOne->Set_Texture(m_pAnimationCom->m_iMotion);
	else
		m_pTextureComTwo->Set_Texture(m_pAnimationCom->m_iMotion);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CEarthSpike * CEarthSpike::Create(LPDIRECT3DDEVICE9 pGraphicDev, float wait, float Posx, float Posy, bool spiketype)
{
	CEarthSpike*	pInstance = new CEarthSpike(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, spiketype, wait)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEarthSpike::Free(void)
{
	CGameObject::Free();
}
