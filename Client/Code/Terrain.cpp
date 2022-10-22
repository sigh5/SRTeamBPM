#include "stdafx.h"
#include "..\Header\Terrain.h"

#include "Export_Function.h"
#include "ControlRoom.h"
#include "Player.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_vDirection({ 0.f, 0.f, 1.f })
{
	
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vCenter = m_pBufferCom->Get_Center();
	
	

	return S_OK;
}

_int CTerrain::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	CScene* pScene = Get_Scene();

	if (pScene->Get_SceneType() != SCENE_TOOLTEST)
	{
		if (m_pCreateControlSphere == false)
		{
			_matrix matWorld;
			m_pTransCom->Get_WorldMatrix(&matWorld);
			D3DXVec3TransformCoord(&m_vCenter, &m_vCenter, &matWorld);

			CScene* pScene = Get_Scene();
			CLayer* pLayer = pScene->GetLayer(L"Layer_Room");

			CGameObject* pGameObject = nullptr;
			pGameObject = CControlRoom::Create(m_pGraphicDev, m_vCenter);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);

			pLayer->Add_ControlRoomList(pGameObject);

			m_pCreateControlSphere = true;
		}
	}
	


	

	

	::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::LateUpdate_Object(void)
{
	

	Engine::CGameObject::LateUpdate_Object();
}

void CTerrain::Render_Obejct(void)
{
	CScene *pScene = Get_Scene();

	_vec3 vPos;
	if(pScene->Get_SceneType() != SCENE_TOOLTEST)
	{ 
		
		CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
		NULL_CHECK(pPlayerTransformCom);

		pPlayerTransformCom->Get_Info(INFO_POS, &vPos);
	}
	_matrix			WorldMatrix, ViewMatrix, ViewMatrixInv, ProjMatrix;

	m_pTransCom->Get_WorldMatrix(&WorldMatrix);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->Set_Raw_Value("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_matrix));
	m_pShaderCom->Set_Raw_Value("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_matrix));
	
	
	if (pScene->Get_SceneType() != SCENE_TOOLTEST)
		m_pShaderCom->Set_Raw_Value("g_vCamPosition", &vPos, sizeof(_vec3));
	else
		m_pShaderCom->Set_Raw_Value("g_vCamPosition", &(ViewMatrixInv.m[3][0]), sizeof(_vec3));


	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", m_iTexIndex);

	m_pShaderCom->Begin_Shader(0);

	m_pBufferCom->Render_Buffer();

	m_pShaderCom->End_Shader();



	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}

_bool CTerrain::Set_SelectGizmo()
{
	return m_pCalculatorCom->PickingTerrainObject(g_hWnd, m_pBufferCom, m_pTransCom);
}

HRESULT CTerrain::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(::Clone_Proto(L"Proto_TerrainTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(::Clone_Proto(L"Proto_TerrainTexture2"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture2", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(::Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(::Clone_Proto(L"Proto_ShaderTerrain"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_ShaderTerrain", pComponent });


	return S_OK;
}


CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain *	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTerrain::Free(void)
{
	CGameObject::Free();
}
