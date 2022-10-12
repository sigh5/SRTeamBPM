#include "stdafx.h"
#include "..\Header\Effect.h"

#include "Export_Function.h"
#include "MyCamera.h"
CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBase_Effect(pGraphicDev)
{
}


CEffect::~CEffect()
{
}

HRESULT CEffect::Ready_Object(const _vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransCom->Set_Pos(_float(30.f), 2.f, _float(30.f));

	m_pTransCom->Set_Pos(vPos.x,vPos.y, vPos.z);


	return S_OK;
}

_int CEffect::Update_Object(const _float & fTimeDelta)
{
	m_fFrame += 8.f * fTimeDelta;

	if (m_fFrame >= 8.f)
	{
		m_fFrame = 0.f;
		return OBJ_DEAD;
	}
	Engine::CBase_Effect::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CEffect::LateUpdate_Object(void)
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

void CEffect::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture((_ulong)m_fFrame);	// �ؽ�ó ���� ������ �켱������ �Ѵ�.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	ADD_CLONE_PROTO(CRcTex, m_pBufferCom, m_mapComponent, ID_STATIC, L"Proto_RcTexCom");
	ADD_CLONE_PROTO(CTexture, m_pTextureCom, m_mapComponent, ID_STATIC, L"Proto_hitbloodTexture");

	ADD_CLONE_PROTO(CTransform, m_pTransCom, m_mapComponent, ID_DYNAMIC, L"Proto_TransformCom");

	return S_OK;
}




CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3&  vPos)
{
	CEffect *	pInstance = new CEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect::Free(void)
{
	CGameObject::Free();
}
