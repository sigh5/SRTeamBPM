#include "stdafx.h"
#include "..\Header\HpPotion.h"

#include "Export_Function.h"

CHpPotion::CHpPotion(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CHpPotion::~CHpPotion()
{
}

HRESULT CHpPotion::Ready_Object(_vec3 vPos)
{

	//	m_fBeetweenMotion = 0.2f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	return S_OK;
}

_int CHpPotion::Update_Object(const _float & fTimeDelta)
{


	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pAnimationCom->Move_Animation(fTimeDelta);

	Set_OnTerrain();//이걸 업데이트 오브젝트보다 나중에 해야함

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

void CHpPotion::Set_Pos(_vec3 vPos)
{
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

HRESULT CHpPotion::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_HpPotionTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HpPotionTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Clone_Proto(L"Proto_AnimationCom"));
	NULL_CHECK_RETURN(m_pAnimationCom, E_FAIL);
	m_pAnimationCom->Ready_Animation(3, 0, 0.2f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_AnimationCom", pComponent });
	return S_OK;
}

void CHpPotion::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

void CHpPotion::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CHpPotion::Render_Obejct(void)
{
	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x30);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pTextureCom->Set_Texture(m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CHpPotion * CHpPotion::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CHpPotion *	pInstance = new CHpPotion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHpPotion::Free(void)
{
	CGameObject::Free();
}