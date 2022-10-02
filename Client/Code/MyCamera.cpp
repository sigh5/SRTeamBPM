#include "stdafx.h"
#include "..\Header\MyCamera.h"

#include "Export_Function.h"


CMyCamera::CMyCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
	
}


CMyCamera::~CMyCamera()
{
}

HRESULT CMyCamera::Ready_Object(const _vec3 * pEye,
	const _vec3 * pAt, 
	const _vec3 * pUp, 
	const _float & fFov,
	const _float & fAspect, 
	const _float & fNear, 
	const _float & fFar)
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

_int CMyCamera::Update_Object(const _float & fTimeDelta)
{
	//Key_Input(fTimeDelta);

	Mouse_Fix();
	
	Target_Renewal();
	
	
	_int iExit = CCamera::Update_Object(fTimeDelta);
	Mouse_Move(fTimeDelta);
	return iExit;
}

void CMyCamera::LateUpdate_Object(void)
{
	
	CCamera::LateUpdate_Object();
}

void CMyCamera::Mouse_Move(const _float& fTimeDelta)
{
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);


	_long		dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		pPlayerTransform->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));

	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_fAngle = dwMouseMove / 10.f;
	}

}

void CMyCamera::Mouse_Fix(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (WINCX+50 >= pt.x  && pt.x > -50 && WINCY+50 >= pt.y && pt.y >= -50)
	{
		ClientToScreen(g_hWnd, &pt);
		
	}
	else
	{
		POINT pt2{ WINCX / 2 , WINCY / 2 };
		//GetCursorPos(&pt2);
		ClientToScreen(g_hWnd, &pt2);
		SetCursorPos(pt2.x,pt2.y);
	}
}


void CMyCamera::Key_Input(const _float & fTimeDelta)
{
	if (Get_DIKeyState(DIK_T) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bCheck = false;

	if (false == m_bFix)
		return;
}

void CMyCamera::Target_Renewal(void)
{
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"TestPlayer", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3	vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;	// ¹æÇâ º¤ÅÍ
	D3DXVec3Normalize(&m_vEye, &m_vEye);
	
	m_vEye.y = m_fCameraHeight; //0.5f;

	m_vEye *= m_fDistance;	// ¹æÇâ º¤ÅÍ

	m_vEye += pPlayerTransform->m_vInfo[INFO_POS];
	m_vAt = pPlayerTransform->m_vInfo[INFO_POS];

	// 
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);
	_vec3		vRight;
	memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_fAngle));
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	m_vAt = m_vEye + vLook;

}

CMyCamera * CMyCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFov, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CMyCamera*		pInstance = new CMyCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMyCamera::Free(void)
{
	CCamera::Free();
}
