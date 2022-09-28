#include "stdafx.h"
#include "..\Header\FatBat.h"

#include "Export_Function.h"
#include "MonsterBullet.h"
#include "Stage.h"

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

	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MonsterTexture2"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MonsterTexture2", pComponent });

	m_iMonsterIndex = 1;
	m_pInfoCom->Ready_CharacterInfo(100, 10, 5.f);
	m_pAnimationCom->Ready_Animation(6, 0, 0.2f);
	if (Posx == 0 && Posy == 0) {}
	else
	{
		Set_TransformPositon(g_hWnd);
	}

	m_iCoolTime = rand() % 100;


	return S_OK;
}

_int CFatBat::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	FatBat_Fly();
	FatBat_Shoot();
	//Set_OnTerrain();
	//������ �ø�

	_vec3		vPlayerPos, vMonsterPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);

	float fMtoPDistance; // ���Ϳ� �÷��̾� ���� �Ÿ�

	fMtoPDistance = sqrtf((powf(vMonsterPos.x - vPlayerPos.x, 2) + powf(vMonsterPos.y - vPlayerPos.y, 2) + powf(vMonsterPos.z - vPlayerPos.z, 2)));

	if (fMtoPDistance > 5.f)
	{
		m_pTransCom->Chase_Target_notRot(&vPlayerPos, m_pInfoCom->Get_InfoRef()._fSpeed, fTimeDelta);
		
	}

	m_pAnimationCom->Move_Animation(fTimeDelta);

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
}

void CFatBat::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CFatBat::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// �ؽ�ó ���� ������ �켱������ �Ѵ�.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void	CFatBat::FatBat_Fly(void)
{
	float TerrainY =	m_pDynamicTransCom->Get_TerrainY1(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pTransCom);
		//L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pTransCom); 
	
	m_pDynamicTransCom->Monster_Fly(m_pTransCom, TerrainY, 3.f);

}

void CFatBat::FatBat_Shoot(void)
{
	++m_iCoolTime;

	if (m_iCoolTime > 150)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		CMonsterBullet* pBullet = CMonsterBullet::Create(m_pGraphicDev, vPos);
		NULL_CHECK(pBullet);

		static_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Push_MonBullet(pBullet);
	
		m_iCoolTime = 0;

	}
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
	CGameObject::Free();
}