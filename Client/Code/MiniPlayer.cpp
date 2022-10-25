#include "stdafx.h"
#include "..\Header\MiniPlayer.h"

#include "Export_Function.h"
#include "AbstractFactory.h"
#include "StaticCamera.h"
#include "Bullet.h"
#include "PiercingBullet.h"


CMiniPlayer::CMiniPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	,  m_vDirection(0.f, 0.f, 0.f)
{
}

CMiniPlayer::~CMiniPlayer()
{
}

HRESULT CMiniPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos = { 30.f, 1.f, 30.f };

	m_pDynamicTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	
	_vec3 vScale = { 0.5f,0.5f,0.5f };
	
	m_pColliderCom->Set_vCenter(&vPos, &vScale);
	m_pColliderCom->Set_HitRadiuos(0.1f);
	
	m_pDynamicTransCom->Set_Scale(&vScale);
	m_pDynamicTransCom->Update_Component(1.f);

	m_pAnimationCom->Ready_Animation(21, 0, 0.2f);
	return S_OK;
}

_int CMiniPlayer::Update_Object(const _float & fTimeDelta)
{
	_matrix matWorld;
	_vec3 vScale;
	vScale = m_pDynamicTransCom->Get_Scale();
	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);
	m_pColliderCom->Set_HitBoxMatrix_With_Scale(&matWorld, vScale);

	Key_Input(fTimeDelta);

	Key_InputReset();
	
	m_fPiercingBulletTime += fTimeDelta;

	m_pAnimationCom->Move_Animation(fTimeDelta);

	if(m_iKillMonster >50)
	{ 
		if (m_bDash) // 대쉬 시간 제한
		{
			m_fDashTimer += 1.f*fTimeDelta;
			m_pDynamicTransCom->Dashing(fTimeDelta, m_pDynamicTransCom, m_vUp, m_vDirection, m_tpType, 1.f);
		}

		if (m_fDashTimer >= .2f)
		{
			m_bDash = false;
			m_fDashTimer = 0.f;

		}
		
	}

	if (m_iKillMonster > 75)
	{
		if (m_fPiercingBulletTime > 10.f)
		{
			m_iPiercingBulletNum++;
			m_fPiercingBulletTime = 0.f;
		}
		if (false == m_bCanPiercing)
			m_bCanPiercing = true;
	}

	m_pDynamicTransCom->Set_Y(1.f);

	if (m_iKillMonster >= 100)
		m_iCreateBulletTime = 2;

	m_fFrame += 1.f*fTimeDelta;

	if (m_fFrame >= m_iCreateBulletTime)
	{
		_int Hitsound = rand() % 3;
		switch (Hitsound)
		{
		case 0:
			::StopSound(SOUND_EFFECT2);
			::PlaySoundW(L"Guitar1.wav", SOUND_EFFECT2, g_fSound * 1.5f);
			break;
		case 1:
			::StopSound(SOUND_EFFECT2);
			::PlaySoundW(L"Guitar2.wav", SOUND_EFFECT2, g_fSound * 1.5f);
			break;

		case 2:
			::StopSound(SOUND_EFFECT2);
			::PlaySoundW(L"Guitar3.wav", SOUND_EFFECT2, g_fSound * 1.5f);
			break;
		}
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		CScene* pScene = Get_Scene();
		CLayer*		pLayer = pScene->GetLayer(L"Layer_GameLogic");
		CGameObject*		pGameObject = nullptr;

		pGameObject = CBullet::Create(m_pGraphicDev,vPos);
		pLayer->Add_EffectList(pGameObject);

		m_fFrame = 0.f;

		int iBulletCreateNum = m_iKillMonster / 5;

		if (iBulletCreateNum >= 5)
			iBulletCreateNum = 4;

		for (int i = 0; i < iBulletCreateNum; ++i)
		{
			pGameObject = CBullet::Create(m_pGraphicDev, vPos);
			pLayer->Add_EffectList(pGameObject);
			static_cast<CBullet*>(pGameObject)->Set_Bullet_Dir(i);
		}
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CMiniPlayer::LateUpdate_Object(void)
{
	//Set_OnTerrain();

	/*CStaticCamera* pCamera = static_cast<CStaticCamera*>(Get_GameObject(L"Layer_Environment", L"StaticCamera"));
	NULL_CHECK(pCamera);

	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pDynamicTransCom->m_vScale.x, m_pDynamicTransCom->m_vScale.y, m_pDynamicTransCom->m_vScale.z);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pDynamicTransCom->Set_WorldMatrix(&(matWorld));*/

	// 빌보드 에러 해결
	
	Engine::CGameObject::LateUpdate_Object();




	CGameObject::LateUpdate_Object();
}

void CMiniPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);
	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);



	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pColliderCom->HitBoxWolrdmat());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	////m_pColliderCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

void CMiniPlayer::Collision_Event()
{
}

HRESULT CMiniPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MiniPlayerTexture", m_mapComponent, ID_STATIC);
	m_pDynamicTransCom = CAbstractFactory<CDynamic_Transform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

void CMiniPlayer::Key_Input(const _float & fTimeDelta)
{
	m_pDynamicTransCom->Get_Info(INFO_LOOK, &m_vDirection);
	m_pDynamicTransCom->Get_Info(INFO_UP, &m_vUp);
	m_pDynamicTransCom->Get_Info(INFO_POS, &m_vPos);


	if (Get_DIKeyState(DIK_W) & 0X80)
	{
		m_tpType = TYPING_W;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));

	}

	if (Get_DIKeyState(DIK_S) & 0X80)
	{
		m_tpType = TYPING_S;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * -m_fSpeed * fTimeDelta));


	}
	if (Get_DIKeyState(DIK_Q) & 0X80)
	{
		/*m_tpType = TYPING_S;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pDynamicTransCom->Move_Pos(&(m_vDirection * -m_fSpeed * fTimeDelta));
*/
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		cout << vPos.x<< " " << vPos.z <<endl ;
	}


	if (Get_DIKeyState(DIK_A) & 0X80)
	{
		m_tpType = TYPING_A;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * m_fSpeed * fTimeDelta));

	}

	if (Get_DIKeyState(DIK_D) & 0X80)
	{
		m_tpType = TYPING_D;
		_vec3	vRight;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Cross(&vRight, &m_vDirection, &m_vUp);
		m_pDynamicTransCom->Move_Pos(&(vRight * -m_fSpeed * fTimeDelta));


	}

	if (::Key_Down(DIK_LSHIFT))
	{
		m_bDash = true;

	
	}
	if (Get_DIKeyState(DIK_SPACE) & 0x80 && 0 < m_iPiercingBulletNum)
	{
		m_iPiercingBulletNum--;
		_vec3 vPos;
		m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);
		CScene* pScene = Get_Scene();
		CLayer*		pLayer = pScene->GetLayer(L"Layer_GameLogic");
		CGameObject*		pGameObject = nullptr;

		pGameObject = CPiercingBullet::Create(m_pGraphicDev, vPos);
		::StopSound(SOUND_CRUSHROCK2);
		::PlaySoundW(L"PiercingBullet.wav", SOUND_CRUSHROCK2, g_fSound * 2.f);
		pLayer->Add_EffectList(pGameObject);
	}


}

void CMiniPlayer::Set_OnTerrain()
{
	_vec3		vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"MiniTerrain4", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	fHeight += 0.5f;

	m_pDynamicTransCom->Set_Pos(vPos.x, fHeight, vPos.z);


}





CMiniPlayer * CMiniPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniPlayer *	pInstance = new CMiniPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniPlayer::Free(void)
{
	CGameObject::Free();
}
