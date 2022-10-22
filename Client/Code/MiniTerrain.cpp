#include "stdafx.h"
#include "..\Header\MiniTerrain.h"

#include "Export_Function.h"
#include "StaticCamera.h"
#include "StageTerrain.h"

CMiniTerrain::CMiniTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_vDirection({ 0.f, 0.f, 1.f })
{
}

CMiniTerrain::~CMiniTerrain()
{
}

HRESULT CMiniTerrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	return S_OK;
}

_int CMiniTerrain::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_fTime += 0.1f *fTimeDelta;


	m_pBufferCom->Ready_Buffer(VTXCNTX, VTXCNTZ, VTXITV, m_fTime);


	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CMiniTerrain::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CMiniTerrain::Render_Obejct(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(::Get_GameObject(L"Layer_Environment",L"StaticCamera"));

	pCam->GetView();

	_matrix			WorldMatrix, ViewMatrix, ViewMatrixInv, ProjMatrix;

	m_pTransCom->Get_WorldMatrix(&WorldMatrix);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->Set_Raw_Value("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_vCamPosition", (_vec3*)&ViewMatrixInv.m[3][0], sizeof(_vec3));

	
	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 8);	// 텍스처 정보 세팅을 우선적으로 한다.
	


	m_pShaderCom->Begin_Shader(0);

	m_pBufferCom->Render_Buffer();

	m_pShaderCom->End_Shader();

	

}

HRESULT CMiniTerrain::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainMoveTex*>(Clone_Proto(L"Proto_TerrainMoveTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainMoveTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_TerrainTexture2"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture2", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(::Clone_Proto(L"Proto_ShaderTerrain"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_ShaderTerrain", pComponent });



	return S_OK;
}

void CMiniTerrain::InitScaleAndPos(_vec3* vScale, _vec3* vPos, _bool bisPaDo)
{
	m_pTransCom->Set_Scale(vScale);
	m_pTransCom->Set_Pos(vPos->x, vPos->y, vPos->z);

	m_bPado = bisPaDo;
}

CMiniTerrain * CMiniTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniTerrain *	pInstance = new CMiniTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniTerrain::Free(void)
{
	CGameObject::Free();
}

