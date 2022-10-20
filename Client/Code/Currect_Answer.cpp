#include "stdafx.h"
#include "..\Header\Currect_Answer.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

// 이 클래스는 폐기되었음 쓰지 마시오.

CCurrect_Answer::CCurrect_Answer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CCurrect_Answer::CCurrect_Answer(const CUI_Base & rhs)
	:CUI_Base(rhs)
{
}

CCurrect_Answer::~CCurrect_Answer()
{
}

HRESULT CCurrect_Answer::Ready_Object(_float fX, _float fY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimationCom->Ready_Animation(6, 0, 0.2f, 6);

	Set_OrthoMatrix(256.f, 256.f, fX, fY);

	_vec3	vScale = { m_fSizeX * 0.4f, m_fSizeY * 0.4f, 1.f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Set_Pos(fX, fY, 0.f);

	ffx = fX;
	ffy = fY;
	
	return S_OK;
}

_int CCurrect_Answer::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	
	cout << ffx << "뭔 차이지" << ffy << endl;

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CCurrect_Answer::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CCurrect_Answer::Render_Obejct(void)
{
	if (!m_bRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

		_matrix		OldViewMatrix, OldProjMatrix;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		_matrix		ViewMatrix;

		ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

		_matrix		matProj;

		Get_ProjMatrix(&matProj);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pTextureCom->Set_Texture(5);//m_pAnimationCom->m_iMotion);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		//m_bRender = true;
	}
	
}

HRESULT CCurrect_Answer::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_CurrectUI_Texture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CCurrect_Answer * CCurrect_Answer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CCurrect_Answer* pInstance = new CCurrect_Answer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(fX, fY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCurrect_Answer::Free(void)
{
	Engine::CGameObject::Free();
}
