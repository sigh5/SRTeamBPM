#include "stdafx.h"
#include "..\Header\LoadingBackFont.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "Change_Stage.h"

CLoadingBackFont::CLoadingBackFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CLoadingBackFont::CLoadingBackFont(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CLoadingBackFont::~CLoadingBackFont()
{
}

HRESULT CLoadingBackFont::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3		vScale = { 1.6f, 1.4f, 0.f };

	m_pTransCom->Set_Scale(&vScale);                        

	m_iRandomText = rand() % 5;

	return S_OK;
}

_int CLoadingBackFont::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CLoadingBackFont::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CLoadingBackFont::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//const Engine::_tchar *pString
	
	_tchar*	pString = L"";

	switch (m_iRandomText)
	{
		case 0:
		{
			pString = L"저녁 메뉴 고민 중..";
			break;
		}

		case 1:
		{
			pString = L"무기 손질하는 중..";
			break;
		}

		case 2:
		{
			pString = L"든든한 한 끼 준비 중..";
			break;
		}

		case 3:
		{
			pString = L"몬스터 자리잡는 중..";
			break;
		}

		case 4:
		{
			pString = L"큐브 깔아두는 중..";
			break;
		}

		case 5:
		{
			pString = L"리듬감을 느끼는 중..";
			break;
		}

	}

	Render_Font(L"LeeSoonSin", pString, &_vec2(523.f, 524.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pTextureCom->Set_Texture(0);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	CGameObject::Render_Obejct();
}

HRESULT CLoadingBackFont::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_LoadingFontBackGround_Texture", m_mapComponent, ID_STATIC);

	return S_OK;
}

CLoadingBackFont * CLoadingBackFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY)
{
	CLoadingBackFont* pInstance = new CLoadingBackFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	pInstance->m_pTransCom->Set_Pos(fX, fY, 0.1f);

	return pInstance;
}

void CLoadingBackFont::Free()
{
	CGameObject::Free();
}
