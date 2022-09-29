#include "stdafx.h"
#include "..\Header\Particle.h"

#include "Export_Function.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CParticle::~CParticle()
{
}

HRESULT CParticle::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransCom->Set_Pos(30.f, 0.f, 10.f);

	//_vec3 scale = { 5.f,5.f,5.f };
	
	//m_pTransCom->Set_Scale(&scale);
	m_pTransCom->Set_Pos(5.f, 2.f, 5.f);
	m_pTransCom->Update_Component(1.f);

	return S_OK;
}

_int CParticle::Update_Object(const _float & fTimeDelta)
{
	m_fFrame += 90.f * fTimeDelta;

	if (m_fFrame >= 90.f)
		m_fFrame = 0.f;

	Engine::CGameObject::Update_Object(fTimeDelta);

	//_matrix		matWorld, matView, matBill;
	//D3DXMatrixIdentity(&matBill);

	//m_pTransCom->Get_WorldMatrix(&matWorld);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;

	//D3DXMatrixInverse(&matBill, 0, &matBill);

	//// 현재 지금 이 코드는 문제가 없지만 나중에 문제가 될 수 있음
	//m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));

	
	Add_RenderGroup(RENDER_PRIORITY, this);

	return _int();
}

void CParticle::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CParticle::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	// PreRender
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, ftoDw(2.5f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, ftoDw(0.2f));
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX, ftoDw(5.0f));

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, ftoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, ftoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, ftoDw(1.0f));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// ~PreRender

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture((_ulong)m_fFrame);	// 텍스처 정보 세팅을 우선적으로 한다.
	m_pGraphicDev->SetFVF(FVF_Particle);
	
	
	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//PostRender
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//~PostRender
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

void CParticle::Test_Snow(BoundingBox * boundingBox, int numPartincles)
{
	_boundingBox = *boundingBox;
	_size = 0.8f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;

	for (int i = 0; i < numPartincles; ++i)
	{
		addParticle();
	}


}

HRESULT CParticle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_EffectTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EffectTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });


	return S_OK;
}

CParticle * CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle *	pInstance = new CParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CParticle::Free(void)
{
	CGameObject::Free();
}
