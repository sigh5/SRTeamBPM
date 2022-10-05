#include "stdafx.h"
#include "..\Header\AnubisThunder.h"
#include "AbstractFactory.h"

#include "Export_Function.h"


CAnubisThunder::CAnubisThunder(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CAnubisThunder::~CAnubisThunder()
{
}

HRESULT CAnubisThunder::Ready_Object(int Posx, int Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pTransformCom->Set_Pos((_float)Posx, 1.f, (_float)Posy);
	}
	m_pTransformCom->
	m_pTransformCom->Compulsion_Update();
	return S_OK;
}

_int CAnubisThunder::Update_Object(const _float & fTimeDelta)
{

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CAnubisThunder::LateUpdate_Object(void)
{
}

void CAnubisThunder::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x40);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
HRESULT			CAnubisThunder::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Anubis_Attack_Texture", m_mapComponent, ID_STATIC);
	//m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CThunderTex>::Clone_Proto_Component(L"Proto_ThunderTexCom", m_mapComponent, ID_STATIC);
	m_pTransformCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CAnubisThunder * CAnubisThunder::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CAnubisThunder*	pInstance = new CAnubisThunder(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnubisThunder::Free(void)
{
	CGameObject::Free();
}
