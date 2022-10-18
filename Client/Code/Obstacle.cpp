#include "stdafx.h"
#include "..\Header\Obstacle.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "Gun_Screen.h"


#include "DynamicCamera.h"



CObstacle::CObstacle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CObstacle::~CObstacle()
{
}

HRESULT CObstacle::InitSetting(_vec2 * vMousPos, const wstring & LayerName, wstring * RoomName)
{
	Set_Layer_Map_Name(LayerName, RoomName);
	CScene* pScene = ::Get_Scene();
	CLayer* pLayer = pScene->GetLayer(LayerName.c_str());

	if ((*vMousPos).x == 0 && (*vMousPos).y == 0) {}
	else
		MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수

	//임시 코드
	_vec3 vCurretPos;
	m_pTransCom->Get_Info(INFO_POS, &vCurretPos);
	vCurretPos.y += 0.5f;
	
	m_pTransCom->Set_Y(vCurretPos.y);

		/*_vec3 vScale = { 3.f,3.f,3.f };
	m_pTransCom->Set_Scale(&vScale);
*/
	//m_pTransCom->Get_Info(INFO_POS, &vCurretPos);


	return S_OK;
}

HRESULT CObstacle::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();


	// set_hit_distance 
	m_pColliderCom->Set_HitRadiuos(1.f);
	m_pColliderCom->Set_vCenter(&vPos, &vScale);

	return S_OK; 
}

_int CObstacle::Update_Object(const _float & fTimeDelta)
{
	switch (m_iTexIndex)
	{
	case OBSTACLE_STREET_LAMP:
		m_bRythmeObstacle = true;
		break;
	case OBSTACLE_WALL_LAMP:
		m_bRythmeObstacle = true;
		break;
	case OBSTACLE_FIRE:
		m_bRythmeObstacle = true;
		break;
	default:
		m_bRythmeObstacle = false;
		break;
	}

	m_fFrame += 1.f* fTimeDelta;

	if (m_fFrame > 1.f)
	{
		m_bControlAnim = false;
		m_fFrame = 0.f;
	}


	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, -1 );

	

	if (m_bRythmeObstacle &&pScene->Get_SceneType() != SCENE_TOOLTEST)
	{
		if (m_iTexIndex != OBSTACLE_FIRE)
		{
			if (static_cast<CGun_Screen*>(Get_GameObject(L"Layer_UI", L"Gun"))->Get_ReadyShot() && !m_bControlAnim)
			{
				m_pAnimationCom->Control_Event_Animation(true);
				m_bControlAnim = true;
			}
		}
		else
		{
			m_pAnimationCom->Move_Animation(fTimeDelta);
			if (static_cast<CGun_Screen*>(Get_GameObject(L"Layer_UI", L"Gun"))->Get_ReadyShot() && !m_bControlAnim)
			{
				m_pAnimationCom->m_iMotion = 0;
				m_bControlAnim = true;
			}
		}
	}
		//지우셈
	else
	{
		m_pAnimationCom->Move_Animation(fTimeDelta);
	}



	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CObstacle::LateUpdate_Object(void)
{
	if (2 == m_iOption)
	{
		Engine::CGameObject::LateUpdate_Object();
		return;
	}
	CScene  *pScene = ::Get_Scene();

	NULL_CHECK_RETURN(pScene, );
	
	if (pScene->Get_SceneType() == SCENE_TOOLTEST)
		return;
	/*CDynamicCamera* pCamera = static_cast<CDynamicCamera*>(Get_GameObject(L"TestLayer", L"DynamicCamera"));
	NULL_CHECK(pCamera);*/

	// SCNENE
	//// 빌보드 에러 해결
	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);

	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

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


	// 빌보드 에러 해결
	Engine::CGameObject::LateUpdate_Object();

}

void CObstacle::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	if (m_bWireFrame)
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(150, 0, 0, 0));
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	CScene *pScene = Get_Scene();
	if (pScene->Get_SceneType() == SCENE_TOOLTEST)
	{
		m_pTextureCom->Set_Texture(m_iTexIndex);
	}
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		if (m_iTexIndex == 0 || m_iTexIndex == 1 || m_iTexIndex == 2 || m_iTexIndex == 3 || m_iTexIndex == 4)
			m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
		else
			m_pTextureCom->Set_Texture(m_iTexIndex);
	}

	if (m_bWireFrame)
	{
		

	}
	m_pBufferCom->Render_Buffer();


	if (m_bWireFrame)
	{
		
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	
}

void CObstacle::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	

}

void CObstacle::MousePostoScreen()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TerrainTexCom", ID_STATIC));

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(m_LayerName.c_str(),
		m_RoomName.c_str(),
		L"Proto_TransformCom", ID_DYNAMIC));

	if (pTerrainTransformCom == nullptr || pTerrainBufferCom == nullptr)
	{
		MSG_BOX("Choose Terrain");
		return;
	}
	//주석지우셈
	_vec3 Temp = m_pCalculatorCom->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);
}

_bool CObstacle::Set_SelectGizmo()
{
	return m_pCalculatorCom->PickingOnTransform_RcTex(g_hWnd, m_pBufferCom, m_pTransCom);
}

HRESULT CObstacle::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	
	CScene *pScene = Get_Scene();
	if (pScene->Get_SceneType() == SCENE_TOOLTEST)
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_fetrues_Texture", m_mapComponent, ID_STATIC);
		return S_OK;
	}
	
	return S_OK;
}


void CObstacle::Set_TextureCom()
{
	
	if (m_iTexIndex== OBSTACLE_CRYSTAL)
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_CrystralAnimTexCom", m_mapComponent, ID_STATIC);
		m_pAnimationCom->Ready_Animation(3, 0, 1.f);
	}
	else if (m_iTexIndex == OBSTACLE_STREET_LAMP)
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_StreetTexCom", m_mapComponent, ID_STATIC);
		m_pAnimationCom->Ready_Animation(2, 0, 2.5f); // 리듬 시간에 맞춰 불빛이 들어옴
	}

	else if (m_iTexIndex == OBJSTACLE_TREE)
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_TreeAnimTexCom", m_mapComponent, ID_STATIC);
		m_pAnimationCom->Ready_Animation(1, 0, 1.f);
	}
	else if (m_iTexIndex == OBSTACLE_WALL_LAMP)
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_WallLampTexCom", m_mapComponent, ID_STATIC);
		m_pAnimationCom->Ready_Animation(2, 0, 2.5f);
	}
	else if(m_iTexIndex ==OBSTACLE_FIRE)
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_FireTexCom", m_mapComponent, ID_STATIC);
		m_pAnimationCom->Ready_Animation(15, 6, 0.15f);
	}
	else
	{
		m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_fetrues_Texture", m_mapComponent, ID_STATIC);
	}

	// ... 추가

}

CObstacle * CObstacle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObstacle*	pInstance = new CObstacle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CObstacle::Free(void)
{
	CGameObject::Free();
}
