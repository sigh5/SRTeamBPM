#include "stdafx.h"
#include "..\Header\Terrain.h"

#include "Export_Function.h"
#include "ControlRoom.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_vDirection({ 0.f, 0.f, 1.f })
{
	
}
//주석지우셈

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
	


	

	

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::LateUpdate_Object(void)
{
	

	Engine::CGameObject::LateUpdate_Object();
}

void CTerrain::Render_Obejct(void)
{

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->LightEnable(3, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(m_iTexIndex);	// 텍스처 정보 세팅을 우선적으로 한다.
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

_bool CTerrain::Set_SelectGizmo()
{
	return m_pCalculatorCom->PickingTerrainObject(g_hWnd, m_pBufferCom, m_pTransCom);
}

HRESULT CTerrain::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_TerrainTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_TerrainTexture2"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture2", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });


	return S_OK;
}


HRESULT CTerrain::SetUp_Material(void)
{
	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	tMtrl.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f,0.2f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	tMtrl.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	// 램버트 확산 조명 공식
	// 명도의 세기 * (L.D * M.D) + L.A * M.A


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
