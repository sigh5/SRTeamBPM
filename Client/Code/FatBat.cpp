#include "stdafx.h"
#include "..\Header\FatBat.h"

#include "Export_Function.h"
#include "MonsterBullet.h"
#include "Stage.h"
#include "AbstractFactory.h"
#include "ObjectMgr.h"

CFatBat::CFatBat(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonsterBase(pGraphicDev)
{
}


CFatBat::~CFatBat()
{
}

HRESULT CFatBat::Ready_Object(int Posx, int Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_MonsterTexture2", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pCalculatorCom = CAbstractFactory<CCalculator>::Clone_Proto_Component(L"Proto_CalculatorCom", m_mapComponent, ID_STATIC);

	m_iMonsterIndex = 1;
	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);
	m_pAnimationCom->Ready_Animation(6, 0, 0.2f);

	_vec3	vScale = { 0.5f,0.5f,0.5f };

	m_pDynamicTransCom->Set_Scale(&vScale);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		Set_TransformPositon(g_hWnd, m_pCalculatorCom);
	}

	m_iCoolTime = rand() % 100;


	return S_OK;
}

_int CFatBat::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom,0);

	FatBat_Fly();
	
	 // 수정 쿨타임 대신 타임
	m_fFrame += 2.f * fTimeDelta;
	if (m_fFrame > 2.f)
	{
		FatBat_Shoot();
		m_fFrame = 0.f;
	}
	// 수정 쿨타임 대신 타임
	//Set_OnTerrain();
	//지형에 올림

	_vec3		vPlayerPos, vMonsterPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pDynamicTransCom->Get_Info(INFO_POS, &vMonsterPos);

	float fMtoPDistance; // 몬스터와 플레이어 간의 거리

	fMtoPDistance = sqrtf((powf(vMonsterPos.x - vPlayerPos.x, 2) + powf(vMonsterPos.y - vPlayerPos.y, 2) + powf(vMonsterPos.z - vPlayerPos.z, 2)));

	if (fMtoPDistance > 5.f)
	{
		m_pDynamicTransCom->Chase_Target_notRot(&vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);
		
	}

	m_pAnimationCom->Move_Animation(fTimeDelta);

	_matrix		matWorld, matView, matBill;
	/*D3DXMatrixIdentity(&matBill);

	m_pDynamicTransCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);


	m_pDynamicTransCom->Set_WorldMatrix(&(matBill * matWorld));*/

	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CFatBat::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CFatBat::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pDynamicTransCom->Get_WorldMatrixPointer());
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

void	CFatBat::FatBat_Fly(void)
{
	float TerrainY =	m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom);
		//L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pDynamicTransCom); 
	
	m_pDynamicTransCom->Monster_Fly(m_pDynamicTransCom, TerrainY, 3.f);

}

void CFatBat::FatBat_Shoot(void)
{

	_vec3 vPos;
	m_pDynamicTransCom->Get_Info(INFO_POS, &vPos);

	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;
	pGameObject = CObjectMgr::GetInstance()->Reuse_MonsterBulltObj(m_pGraphicDev, vPos);
	NULL_CHECK_RETURN(pGameObject, );
	pMyLayer->Add_GameObjectList(pGameObject);
}

CFatBat * CFatBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CFatBat*	pInstance = new CFatBat(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFatBat::Free(void)
{
	CMonsterBase::Free();
}
