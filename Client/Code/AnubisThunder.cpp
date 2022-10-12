#include "stdafx.h"
#include "..\Header\AnubisThunder.h"
#include "AbstractFactory.h"

#include "Export_Function.h"
#include "MyCamera.h"

CAnubisThunder::CAnubisThunder(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CAnubisThunder::~CAnubisThunder()
{
}

HRESULT CAnubisThunder::Ready_Object(int Posx, int Posy)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Rotation(ROT_Y, 90.f);

	if (Posx == 0 && Posy == 0) {}
	else
	{
		m_pTransformCom->Set_Pos((_float)Posx, 1.f, (_float)Posy);
	}

	m_pAnimationCom->Ready_Animation(8, 0, 0.1f);

	m_pTransformCom->Compulsion_Update();
	return S_OK;
}

_int CAnubisThunder::Update_Object(const _float & fTimeDelta)
{
	m_pAnimationCom->Move_Animation(fTimeDelta);

	LateUpdate_Object();
	Render_Obejct();

	m_pTransformCom->Update_Component(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CAnubisThunder::LateUpdate_Object(void)
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
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransformCom->Set_WorldMatrix(&(matWorld));

	Engine::CGameObject::LateUpdate_Object();
}

void CAnubisThunder::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
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

}

void				CAnubisThunder::Set_Direction(_vec3* _Dirvec)
{
	//_matrix matworld;
	//m_pTransformCom->Get_WorldMatrix(&matworld);

	////memcpy(&matworld._33, &_Dirvec, sizeof(_vec3));

	//_vec3 vUp, vLook, vRight;
	//vLook = *_Dirvec;

	//D3DXVec3Normalize(&vLook, &vLook);

	//vUp = { 0.f, 1.f, 0.f };

	//D3DXVec3Cross(&vRight, &vUp, &vLook);

	//D3DXVec3Normalize(&vRight, &vRight);

	//D3DXVec3Cross(&vUp, &vLook, &vRight);

	//D3DXVec3Normalize(&vUp, &vUp);

	//memcpy(&matworld._11, &vRight, sizeof(_vec3));
	//memcpy(&matworld._22, &vUp, sizeof(_vec3));
	//memcpy(&matworld._33, &vLook, sizeof(_vec3));

	//m_pTransformCom->Set_WorldMatrix(&matworld);
	////m_pTransformCom->
	////???

	//_matrix matworld;
	//m_pTransformCom->Get_WorldMatrix(&matworld);

	//_vec3 vUp, vLook, vRight;

	//vLook = *_Dirvec;

	//D3DXVec3Normalize(&vLook, &vLook);

	//vUp = { 0.f, 1.f, 0.f };

	//D3DXVec3Cross(&vRight, &vUp, &vLook);

	//D3DXVec3Normalize(&vRight, &vRight);

	//D3DXVec3Cross(&vUp, &vLook, &vRight);

	//D3DXVec3Normalize(&vUp, &vUp);

	//_vec3		vAxis;
	//_matrix		matRot;

	//// D3DXMatrixRotationAxis : 임의의 축회전 행렬을 만들어주는 함수
	//D3DXMatrixRotationAxis(&matRot,
	//	D3DXVec3Cross(&vAxis, &vUp, &vLook),
	//	acosf(D3DXVec3Dot(D3DXVec3Normalize(&vLook, &vLook),
	//		D3DXVec3Normalize(&vUp, &vUp))));


	//matworld =  matRot;
	//m_pTransformCom->Set_WorldMatrix(&(matworld));
	//m_pTransformCom->Update_Component(1.0f);

}

HRESULT			CAnubisThunder::Add_Component(void)
{
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_Anubis_Thunder_Texture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	//m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pBufferCom = CAbstractFactory<CThunderTex>::Clone_Proto_Component(L"Proto_ThunderTexCom", m_mapComponent, ID_STATIC);
	m_pTransformCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

CAnubisThunder * CAnubisThunder::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CAnubisThunder*	pInstance = new CAnubisThunder(pGraphicDev);


	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnubisThunder::Free(void)
{
	CGameObject::Free();
}
