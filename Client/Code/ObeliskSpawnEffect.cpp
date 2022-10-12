#include "stdafx.h"
#include "..\Header\ObeliskSpawnEffect.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"

CObeliskSpawnEffect::CObeliskSpawnEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CObeliskSpawnEffect::~CObeliskSpawnEffect()
{
}

HRESULT CObeliskSpawnEffect::Ready_Object(const _vec3 & vPos)
{
	Add_Component();

	m_pAnimationCom->Ready_Animation(8, 0, 0.15f);

	m_pTransCom->Set_Pos(vPos.x, vPos.y*0.5f, vPos.z);
	m_pTransCom->Set_Scale(&_vec3(2.f, 2.f, 2.f));
	m_pTransCom->Update_Component(1.f);
	return S_OK;
}

_int CObeliskSpawnEffect::Update_Object(const _float & fTimeDelta)
{
	if (m_pAnimationCom->m_iMotion == m_pAnimationCom->m_iMaxMotion)
	{
		return OBJ_DEAD;
	}

	m_pAnimationCom->Move_Animation(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CObeliskSpawnEffect::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransCom->Set_WorldMatrix(&(matWorld));

	Engine::CGameObject::LateUpdate_Object();
}

void CObeliskSpawnEffect::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CObeliskSpawnEffect::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Obelisk_Summon_Texture", m_mapComponent, ID_STATIC);

	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);
	
	return S_OK;
}

CObeliskSpawnEffect * CObeliskSpawnEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos)
{
	CObeliskSpawnEffect*	pInstance = new CObeliskSpawnEffect(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CObeliskSpawnEffect::Free(void)
{
	CGameObject::Free();
}
