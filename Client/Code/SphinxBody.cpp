#include "stdafx.h"
#include "..\Header\SphinxBody.h"

#include "Export_Function.h"
#include "AbstractFactory.h"

CSphinxBody::CSphinxBody(LPDIRECT3DDEVICE9 pGraphicDev)
		:CGameObject(pGraphicDev)
{
}


CSphinxBody::~CSphinxBody()
{
}

HRESULT CSphinxBody::Ready_Object(float Posx, float Posy, float Size)
{
	m_pTransformCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Sphinx_body_Texture", m_mapComponent, ID_STATIC);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pTransformCom->Set_Pos((float)Posx, Size, (float)Posy);
	}
	m_pTransformCom->Set_Scale(&_vec3(Size, Size, 1.f));
	return S_OK;
}

_int CSphinxBody::Update_Object(const _float & fTimeDelta)
{

	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CSphinxBody::LateUpdate_Object(void)
{

}

void CSphinxBody::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CSphinxBody * CSphinxBody::Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx, float Posy, float Size)
{
	CSphinxBody*	pInstance = new CSphinxBody(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy, Size)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSphinxBody::Free(void)
{
	CGameObject::Free();
}
