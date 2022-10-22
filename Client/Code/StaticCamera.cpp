#include "stdafx.h"
#include "..\Header\StaticCamera.h"

#include "Export_Function.h"
#include "MiniPlayer.h"

// 현욱 미니스테이지 전용 카메라

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}


CStaticCamera::~CStaticCamera()
{
}

HRESULT CStaticCamera::Ready_Object(const _vec3* pEye, 
	const _vec3* pAt, 
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect ,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CCamera::Update_Object(fTimeDelta);

	if (m_bPlayerHit)
	{
		m_fFrame += 0.2f *fTimeDelta;
		m_itemp *= -1;
		Target_Renewal();
		m_vEye.y = m_vEye.y + (_float(m_itemp)*0.1f* fTimeDelta);

		if (m_fFrame >= 0.2f)
		{
			m_fFrame = 0.f;
			m_bPlayerHit = false;
		}
	}
	else
	{
		Target_Renewal();
	}

	Mouse_Move(fTimeDelta);
	Mouse_Fix();

	return iExit;
}

void CStaticCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CStaticCamera::Set_Shaking()
{
	m_bPlayerHit = true;
}


CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (float)WINCX / WINCY*/, const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/)
{
	CStaticCamera*		pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}


void CStaticCamera::Target_Renewal(void)
{
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3	vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;	// 방향 벡터
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye.y = m_fCameraHeight;
	m_vEye *= m_fDistance;	// 방향 벡터

	m_vEye += pPlayerTransform->m_vInfo[INFO_POS];
	m_vAt = pPlayerTransform->m_vInfo[INFO_POS];

	m_vAt = m_vEye + vLook;

}

void CStaticCamera::Mouse_Move(const _float & fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		pPlayerTransform->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
		m_iBillBoardDir = (_int)(dwMouseMove / 10.f) *fTimeDelta;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_fAngle = D3DXToRadian(dwMouseMove / 10.f) * fTimeDelta;
		pPlayerTransform->Rotation(ROT_X, m_fAngle);
	}

}

void CStaticCamera::Mouse_Fix()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	POINT	pt2{ (WINCX >> 1) - 16 , (WINCY >> 1) - 14 };

	ClientToScreen(g_hWnd, &pt2);
	SetCursorPos(pt2.x, pt2.y);
}

void CStaticCamera::Free(void)
{
	CCamera::Free();
}
