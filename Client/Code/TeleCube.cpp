#include "stdafx.h"
#include "TeleCube.h"

#include "Export_Function.h"
#include "Terrain.h"
#include "AbstractFactory.h"
#include "ControlRoom.h"
#include "Change_Stage.h"

CTeleCube::CTeleCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTeleCube::~CTeleCube()
{
}

HRESULT CTeleCube::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	// set_hit_distance 
	m_pColliderCom->Set_HitRadiuos(2.f);
	m_pColliderCom->Set_vCenter(&vPos, &vScale);


	return S_OK;
}

_int CTeleCube::Update_Object(const _float & fTimeDelta)
{

	if (m_iForceSceneReturn == SCENE_CHANGE_RETRURN)
		return SCENE_CHANGE_RETRURN;

	if (m_bTest)
	{
		CScene*pScene = ::Get_Scene();
		CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
		NULL_CHECK_RETURN(pLayer, );
		CGameObject *pGameObject = nullptr;

		pGameObject = pLayer->Get_GameObject(L"Player");

		CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
		pTransform->Set_Pos(510.f, 2.f, 510.f);
		pTransform->Update_Component(1.f);
		pScene->Set_SceneChane(true);
		::Set_SaveScene(pScene);
		CScene*		pChangeScene = CChange_Stage::Create(m_pGraphicDev, 79);
		NULL_CHECK_RETURN(pScene, );
		::Change_Scene(pScene, pChangeScene);
		m_iForceSceneReturn = SCENE_CHANGE_RETRURN;
		
		m_bTest = false;
		return SCENE_CHANGE_RETRURN;
	}

	
	


	m_pColliderCom->Set_HitBoxMatrix(&(m_pTransCom->m_matWorld));


	if (m_bCollisionCheck)
	{
		CScene* pScene = Get_Scene();
		CLayer* pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
		for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
		{
			for (auto iter : *(pLayer->Get_TeleCubeList(i)))
				dynamic_cast<CTeleCube*>(iter)->Set_Active(true);
		}

		m_fActiveTimer += 1.f*fTimeDelta;
	}


	if (m_fActiveTimer >= 5.f)
	{
		m_bCollisionCheck = false;
		m_fActiveTimer = 0.f;
	}



	m_fAlphaTimer += 1.f*fTimeDelta;

	if (m_fAlphaTimer >= 1.f)
	{
		m_bAlpha = !m_bAlpha;
		m_fAlphaTimer = 0.f;
	}



	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;

}

void CTeleCube::LateUpdate_Object()
{
	//if (!m_bOnce)
	{
		m_bSetActive = true;
		m_bOnce = true;
	}

}

void CTeleCube::Render_Obejct(void)
{
	if (m_bSetActive == true)
		return;



	if (m_iOption == (_int)CUBE_START_TELE)
	{
		if (m_bAlpha)
			return;
		_matrix			WorldMatrix, ViewMatrix, ViewMatrixInv, ProjMatrix;
		m_pTransCom->Get_WorldMatrix(&WorldMatrix);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &ViewMatrix);
		D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

		m_pShaderCom->Set_Raw_Value("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_matrix));
		m_pShaderCom->Set_Raw_Value("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_matrix));
		m_pShaderCom->Set_Raw_Value("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_matrix));


		m_pShaderCom->Set_Bool("g_RenderOn", false);

		m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", m_iTexIndex);

		m_pShaderCom->Begin_Shader(0);

		m_pBufferCom->Render_Buffer();

		m_pShaderCom->End_Shader();


		// ~Hit Box 
		return;
	}
	else if (m_iOption == _int(CUBE_END_TELE))
	{

		if (m_bAlpha)
			return;
		_matrix			WorldMatrix, ViewMatrix, ViewMatrixInv, ProjMatrix;
		m_pTransCom->Get_WorldMatrix(&WorldMatrix);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &ViewMatrix);
		D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
		
		m_pShaderCom->Set_Raw_Value("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_matrix));
		m_pShaderCom->Set_Raw_Value("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_matrix));
		m_pShaderCom->Set_Raw_Value("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_matrix));
		
		
		m_pShaderCom->Set_Bool("g_RenderOn", true);

		m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", m_iTexIndex);

		m_pShaderCom->Begin_Shader(0);

		m_pBufferCom->Render_Buffer();

		m_pShaderCom->End_Shader();

	}


}

void CTeleCube::Collision_Event()
{
	if (m_bSetActive == true)
		return;
	
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");

	if (m_iOption == (_int)CUBE_START_TELE)
	{
		// 스타트는 히트박스 그려주고있음
		CCollider *pCollider = dynamic_cast<CCollider*>(pGameObject->Get_Component(L"Proto_ColliderCom", ID_STATIC));

		if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
		{
			CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
			pLayer = pScene->GetLayer(L"Layer_CubeCollsion");

			CTeleCube* pTeleCube = dynamic_cast<CTeleCube*>(pLayer->Get_PreRoomTeleCube());

			if (pTeleCube == nullptr)
				return;

			CTransform* sour = dynamic_cast<CTransform*>(pTeleCube->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
			pLayer->m_iRestRoom++; 
			_vec3 vPos;
			sour->Get_Info(INFO_POS, &vPos);

			_vec3 vRight,vLook,vAngle;
			m_pTransCom->Get_Info(INFO_RIGHT, &vRight);
			m_pTransCom->Get_Info(INFO_LOOK, &vLook);
			vAngle = m_pTransCom->Get_Angle();

			D3DXVec3Normalize(&vRight, &vRight);
			D3DXVec3Normalize(&vLook, &vLook);
			
			_matrix matWorld;

			m_pTransCom->Get_WorldMatrix(&matWorld);
			
			pTransform->Set_Pos(vPos.x , vPos.y, vPos.z);
			pTransform->Rotation(ROT_Y, vAngle.y);
			pTransform->Update_Component(1.f);
			
			CScene* pScene = Get_Scene();
			CLayer* pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
			for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
			{
				for (auto iter : *(pLayer->Get_TeleCubeList(i)))
					dynamic_cast<CTeleCube*>(iter)->Set_Active(true);
			}
			m_bCollisionCheck = true;

		}

	}

	else if (m_iOption == (_int)CUBE_END_TELE)
	{
		CCollider *pCollider = dynamic_cast<CCollider*>(pGameObject->Get_Component(L"Proto_ColliderCom", ID_STATIC));

		if (m_pColliderCom->Check_CollisonUseCollider(m_pColliderCom, pCollider))
		{
			Get_Scene()->GetLayer(L"Layer_GameLogic")->Clear_ColliderMonster();
			Get_Scene()->GetLayer(L"Layer_GameLogic")->Delete_GhulList();

			CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

			pLayer = pScene->GetLayer(L"Layer_CubeCollsion");

			if (pLayer->m_iRestRoom < 0)
			{
				// 여기서 최종맵으로 가는 코드 넣으면됀다

				m_bTest = true;
				for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
				{
					for (auto iter : *(pLayer->Get_TeleCubeList(i)))
						dynamic_cast<CTeleCube*>(iter)->Set_Active(true);
				}
				return;
			}
			vector<CGameObject*> temp = *pLayer->GetRestCube();
			_int iCurrentRoomIndex = pLayer->m_iRoomIndex;
			_int iRandomNum = pLayer->m_iRestRoom--; //rand() %
			if (pLayer->m_iRestRoom >= 0)
				pLayer->Save_CurrentRoom(this);

			CTransform* sour = dynamic_cast<CTransform*>(temp[iRandomNum]->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
			_vec3 vPos;
			sour->Get_Info(INFO_POS, &vPos);

			_vec3 vAngle;
			vAngle =sour->Get_Angle();
		
			pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
			pTransform->Rotation(ROT_Y, vAngle.y);
			pTransform->Update_Component(1.f);
			CScene* pScene = Get_Scene();
			CLayer* pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
			for (int i = 0; i < TELEPORT_CUBE_LIST_END; ++i)
			{
				for (auto iter : *(pLayer->Get_TeleCubeList(i)))
					dynamic_cast<CTeleCube*>(iter)->Set_Active(true);
			}
			m_bCollisionCheck = true;
		}
	}

}

void CTeleCube::Set_ForceScene(_int iNum)
{
	 m_iForceSceneReturn = iNum; 
	
	 if (iNum == 0)
		m_bTest = false; 
}

void CTeleCube::Random_ResurrectionRoom()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene,);
	CLayer * pLayer = pScene->GetLayer(L"Layer_CubeCollsion");
	NULL_CHECK_RETURN(pLayer, );

	pLayer->Clear_Stack();
	pLayer->m_iRestRoom = 5;

	random_shuffle((*pLayer->GetRestCube()).begin(), (*pLayer->GetRestCube()).end());

	pLayer->m_iRoomIndex = pLayer->m_iRestRoom--;
	CGameObject* pFirstCubeObj = (*pLayer->GetRestCube())[pLayer->m_iRoomIndex];

	pLayer = pScene->GetLayer(L"Layer_GameLogic");
	CGameObject* pPlayer = pLayer->Get_GameObject(L"Player");

	CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pPlayer->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));
	CTransform* pFirstCubeTransform = dynamic_cast<CTransform*>(pFirstCubeObj->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	_vec3 vFirstCubePos;
	pFirstCubeTransform->Get_Info(INFO_POS, &vFirstCubePos);

	pTransform->Set_Pos(vFirstCubePos.x + 5.f, vFirstCubePos.y, vFirstCubePos.z + 5.f);

	pTransform->Update_Component(1.f);

	pLayer->Reset_Monster();
}

HRESULT CTeleCube::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MapCubeTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	m_pShaderCom = CAbstractFactory<CShader>::Clone_Proto_Component(L"Proto_ShaderCube", m_mapComponent, ID_DYNAMIC);

	return S_OK;
}

CTeleCube * CTeleCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTeleCube*	pInstance = new CTeleCube(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTeleCube::Free()
{
	CGameObject::Free();
}


