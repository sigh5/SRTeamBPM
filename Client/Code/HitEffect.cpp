#include "stdafx.h"
#include "..\Header\HitEffect.h"


#include "Export_Function.h"
#include "MyCamera.h"


CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBase_Effect(pGraphicDev)
{
}

CHitEffect::~CHitEffect()
{
}

HRESULT CHitEffect::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	return S_OK;
}


_int CHitEffect::Update_Object(const _float & fTimeDelta)
{
	
	if (m_pAnimationCom->m_iMotion == m_pAnimationCom->m_iMaxMotion)
	{
		return OBJ_DEAD;
	}
	m_pAnimationCom->Move_Animation(fTimeDelta);
	
	Engine::CBase_Effect::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;

}

void CHitEffect::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

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

	Engine::CBase_Effect::LateUpdate_Object();

}

void CHitEffect::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CHitEffect::Set_Effect_INFO(_int iEffcet_Owner, _int iStartAnimNum, _int iAnimMax, _float fIntervel)
{
	CBase_Effect::Set_Effect_INFO(iEffcet_Owner, iStartAnimNum, iAnimMax, fIntervel);
	

	switch(iEffcet_Owner)
	{
	case OWNER_SPHINX:
		ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_hitbloodTexture3");
		break;
	case OWNER_FLYHEAD:
		ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_hitbloodTexture3");
		break;

	
	default:
		ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_hitbloodTexture");
		break;
	}


	m_pAnimationCom->Ready_Animation(iAnimMax, iStartAnimNum, fIntervel);


	return S_OK;

}

HRESULT CHitEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	ADD_CLONE_PROTO(CRcTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_RcTexCom");
	ADD_CLONE_PROTO(CAnimation, m_pAnimationCom, m_mapComponent, ID_STATIC, L"Proto_AnimationCom");
	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");

	return S_OK;
}

CHitEffect * CHitEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CHitEffect *	pInstance = new CHitEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHitEffect::Free(void)
{
	CBase_Effect::Free();
}
