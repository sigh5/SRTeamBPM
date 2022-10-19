#include "stdafx.h"
#include "..\Header\BackGround.h"

#include "Export_Function.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_vOriginScale = { 2.0f, 2.0f, 2.0f };

	return S_OK;
}

Engine::_int CBackGround::Update_Object(const _float& fTimeDelta)
{
	
	m_fFrame += 1.f*fTimeDelta;
	
	
	_vec3 vecScale = { 2.0f, 2.0f, 2.0f };

	
	if (m_fFrame >= 0.1f)
	{
		if (m_iTempNum % 2 == 0)
		{
			vecScale = { 2.1f,2.1f,2.1f };
		}
		else
		{
			vecScale = { 2.0f, 2.0f, 2.0f };
		}
		++m_iTempNum;
		m_fFrame = 0;
	}




	m_pTransCom->Set_Scale(&vecScale);

	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CBackGround::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CBackGround::Render_Obejct(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix		IdentityMatrix = *D3DXMatrixIdentity(&IdentityMatrix);

	_matrix		matWorld; 
	(m_pTransCom->Get_WorldMatrix(&matWorld));

	D3DXMatrixTranspose(&matWorld, &matWorld);






	if (FAILED(m_pShaderCom->Set_Raw_Value("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_Raw_Value("g_ViewMatrix", &IdentityMatrix, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_Raw_Value("g_ProjMatrix", &IdentityMatrix, sizeof(_matrix))))
		return;

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);

	m_pShaderCom->Begin_Shader(0);

	m_pBufferCom->Render_Buffer();

	m_pShaderCom->End_Shader();
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround *	pInstance = new CBackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

void CBackGround::Free(void)
{
	CGameObject::Free();
}

HRESULT CBackGround::Add_Component(void)
{
	CComponent* pComponent = nullptr;
		
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_LogoTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LogoTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_ShaderRectCom"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ShaderRectCom", pComponent });

	
	return S_OK;
}
