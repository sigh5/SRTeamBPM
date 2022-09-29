#include "stdafx.h"
#include "..\Header\Bullet.h"
#include "Export_Function.h"

#include "AbstractFactory.h"
#include "ObjectMgr.h"


CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBaseBullet(pGraphicDev)
{
}


CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	//m_pTransCom->Set_Scale(0.1f, 0.1f, 0.1f);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	//
	//// Test Tool 용
	////CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	////NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	//	
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	pPlayerTransformCom->Get_Info(INFO_LOOK, &m_MoveDir);

	// 추후에 카메라랑 플레이어 합치면 카메라 좌표 따기 위해 써둔 코드. 건들지 말아줭...
	/*_matrix		matCamView;
	D3DXMatrixIdentity(&matCamView);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamView);
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &matCamView);
	m_vCamLook = { matCamWorld._11, matCamWorld._22, matCamWorld._33 };*/




	return S_OK;
}

_int CBullet::Update_Object(const _float & fTimeDelta)
{	
	m_fFrame += 2.f * fTimeDelta;

	if (m_fFrame > 2.f)
	{
		CObjectMgr::GetInstance()->Collect_BulletObj(this);
		m_fFrame = 0.f;
		return 5;
	}



	m_pTransCom->Move_Pos(&(m_MoveDir * 50.f * fTimeDelta));


	Engine::CGameObject::Update_Object(fTimeDelta);
	
	Add_RenderGroup(RENDER_NONALPHA, this);
		
	return 0;
}

void CBullet::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CBullet::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture(0);
	m_pCubeTexCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet::Add_Component(void)
{
	//CComponent* pComponent = nullptr;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_BulletTexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCubeTexCom = CAbstractFactory<CCubeTex>::Clone_Proto_Component(L"Proto_CubeTexCom", m_mapComponent, ID_STATIC);


	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBullet *	pInstance = new CBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBullet::Free(void)
{
	Engine::CBaseBullet::Free();
}
