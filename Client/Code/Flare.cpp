#include "stdafx.h"
#include "..\Header\Flare.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "MyCamera.h"
#include "FireWorks.h"

CFlare::CFlare(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CFlare::~CFlare()
{
}

HRESULT CFlare::Ready_Object(_vec3 vPos)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Flaretexture", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_DynamicTransformCom", m_mapComponent, ID_DYNAMIC);
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);

	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pAnimationCom->Ready_Animation(5, 0, 1.5f);

	_vec3 vScale = {0.7f, 0.7f, 0.7f };
	m_pTransCom->Set_Scale(&vScale);

	m_pTransCom->Update_Component(1.f);
	return S_OK;
}

_int CFlare::Update_Object(const _float & fTimeDelta)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");
	if (false == m_bSound)
	{
		::StopSound(SOUND_EFFECT2);
		::PlaySoundW(L"Flare.wav", SOUND_EFFECT2, 0.5f);
		m_bSound = true;
	}
	m_pAnimationCom->Move_Animation(fTimeDelta);
	m_pTransCom->Move_Pos(&(_vec3(0.f, 1.f, 0.f) * 20.f * fTimeDelta));

	if (m_pAnimationCom->m_iMaxMotion == m_pAnimationCom->m_iMotion)
	{
		CGameObject* pGameObject = nullptr;
		pGameObject = CFireWorks::Create(m_pGraphicDev, m_pTransCom->m_vInfo[INFO_POS]);
		pMyLayer->Add_EffectList(pGameObject);

		m_bDead = true;
		return OBJ_DEAD;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CFlare::LateUpdate_Object(void)
{
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

void CFlare::Render_Obejct(void)
{
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

CFlare * CFlare::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CFlare *	pInstance = new CFlare(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFlare::Free(void)
{
	Engine::CGameObject::Free();
}
