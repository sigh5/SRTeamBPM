#include "stdafx.h"
#include "..\Header\Anubis.h"

#include "Export_Function.h"

CAnubis::CAnubis(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CAnubis::~CAnubis()
{
}

HRESULT CAnubis::Ready_Object(int Posx, int Posy)
{
	m_fSpeed = 5.f;

	m_Info._Hp = 100;
	m_Info._AttackPower = 10;
	m_Info._MonsterIndex = 0;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		Set_TransformPositon();
	}
	//_vec3 vScale = { 0.5f,0.5f,0.5f };
	//m_pTransCom->Set_Scale(&vScale);
	return S_OK;
}

_int CAnubis::Update_Object(const _float & fTimeDelta)
{
	//플레이어를 추적해야 함
	//테스트 해서 버벅거리는 현상 잡아야 함
	//플레이어와 일정거리 이상 가까워 지면 정지해야 함
	//빌보드
	m_fTimeDelta = fTimeDelta;

	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

#ifdef _DEBUG

	CTexture*	pComponent = nullptr;
	if (m_iPreIndex != m_Info._MonsterIndex)
	{
		m_iPreIndex = m_Info._MonsterIndex;
		switch (m_Info._MonsterIndex)
		{
		case 0:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture", pComponent });
			break;

		case 1:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture2"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture2", pComponent });
			break;

		case 2:

			break;
		}
	}

#else
	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	Set_OnTerrain();
	//지형에 올림

	_vec3		vPlayerPos, vMonsterPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);

	float fMtoPDistance; // 몬스터와 플레이어 간의 거리

	fMtoPDistance = sqrtf((powf(vMonsterPos.x - vPlayerPos.x, 2) + powf(vMonsterPos.y - vPlayerPos.y, 2) + powf(vMonsterPos.z - vPlayerPos.z, 2)));

	if (fMtoPDistance > 5.f)
	{
		m_pTransCom->Chase_Target_notRot(&vPlayerPos, m_fSpeed, m_fTimeDelta);

		m_pAnimationCom->Move_Animation(fTimeDelta);
	}
	else
	{
		m_pAnimationCom->m_iMotion = 0;
	}
#endif
	
	//if (m_fBeetweenMotion < m_fMotionChangeCounter)
	//{//모션 체인지 카운터에 timedelta가 누적된다. beetweenmotion보다 커지면 모션을 바꾸고 0으로 초기화한다.
	//	m_fMotionChangeCounter = 0;
	//	if(m_iMotion < 6)
	//		++m_iMotion;
	//	else
	//		m_iMotion = 1;
	//}
	//else
	//{
	//	m_fMotionChangeCounter += m_fTimeDelta;
	//}
	/*}
	else
	{
	m_iMotion = 0;
	}*/

	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);


	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));

	Add_RenderGroup(RENDER_ALPHA, this);

	return _int();
}

void CAnubis::LateUpdate_Object(void)
{


	Engine::CGameObject::LateUpdate_Object();
}

void CAnubis::Render_Obejct(void)
{
	//알파블렌딩

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CAnubis::Set_Pos(_vec3 vPos)
{
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

HRESULT CAnubis::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	m_pAnimationCom->Ready_Animation(6, 1, 0.2f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });

	return S_OK;
}

void CAnubis::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

bool CAnubis::Set_TransformPositon()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"TestLayer", L"TestMap", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );


	_vec3 Temp = m_pCalculatorCom->PickingOnTerrainCube(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);
}

bool CAnubis::Set_SelectGizmo()
{
	if (m_pCalculatorCom->PickingOnTransform_Monster(g_hWnd, m_pBufferCom, m_pTransCom))
		return true;


	return false;
}

CAnubis* CAnubis::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CAnubis*	pInstance = new CAnubis(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CAnubis::Free(void)
{
	CGameObject::Free();
}
