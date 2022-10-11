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
	Mouse_Fix();
	Key_Input(fTimeDelta);

	Mouse_Move(fTimeDelta);
	
	if (m_bExecution)
	{
		m_fCameraOriginHeight = m_fCameraHeight;
		m_fFrame += 0.2f *fTimeDelta;
		
		Excution_Renewal(fTimeDelta);

		if (m_fFrame >= 0.2f)
		{
			m_fCameraHeight = m_fCameraOriginHeight;
			m_fFrame = 0.f;
			m_bExecution = false;
			m_bSoundCheck = false;
		}
	}

	else if (m_bPlayerHit)
	{
		m_fFrame += 0.2f *fTimeDelta;
		m_itemp *= -1;
		m_vEye.y = m_vEye.y + (_float(m_itemp)*0.1f* fTimeDelta);
		
		if (m_fFrame >= 0.2f)
		{
			m_fFrame = 0.f;
			m_bPlayerHit = false;	
		}
	}

	else
	{
		Target_Renewal(fTimeDelta);
	}


	_int iExit = CCamera::Update_Object(fTimeDelta);
	
	return iExit;
}

void CMyCamera::LateUpdate_Object(void)
{
	
	CCamera::LateUpdate_Object();
}

void CMyCamera::Mouse_Move(const _float& fTimeDelta)
{
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
 	NULL_CHECK(pPlayerTransform);


	_long		dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		pPlayerTransform->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
		m_iBillBoardDir =(_int)(dwMouseMove / 10.f * fTimeDelta);
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_fAngle = D3DXToRadian(dwMouseMove / 10.f) * fTimeDelta;
		pPlayerTransform->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f)*fTimeDelta);
	}

}

void CMyCamera::Mouse_Fix(void)
{
	//POINT	pt{};
	//GetCursorPos(&pt);
	//ScreenToClient(g_hWnd, &pt);

	
	
	//int Mouse_speed = 0;

	//SystemParametersInfo(SPI_GETMOUSESPEED, 0, (PVOID)&Mouse_speed, 0);

	int Mouse_speed = 3;

	SystemParametersInfo(SPI_SETMOUSESPEED,
		0,
		(LPVOID)Mouse_speed,
		SPIF_UPDATEINIFILE ||
		SPIF_SENDCHANGE ||
		SPIF_SENDWININICHANGE);


	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	//  마우스 고정
	if (pt.x >(WINCX >> 1) +200 || pt.x <(WINCX>>1) -200)
	{
		POINT	pt2{ WINCX >> 1 , WINCY >> 1 };

		ClientToScreen(g_hWnd, &pt2);
		SetCursorPos(pt2.x, pt2.y);
	}
	
	

}


void CMyCamera::Key_Input(const _float & fTimeDelta)
{
	//if (Get_DIKeyState(DIK_T) & 0x80)
	//{
	//	if (m_bCheck)
	//		return;

	//	m_bCheck = true;

	//	if (m_bFix)
	//		m_bFix = false;
	//	else
	//		m_bFix = true;
	//}
	//else
	//	m_bCheck = false;

	//if (false == m_bFix)
	//	return;

	
	


}

void CMyCamera::Target_Renewal(const _float& fTimeDelta)
{
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3	vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;	// 방향 벡터
	D3DXVec3Normalize(&m_vEye, &m_vEye);
	
	m_vEye.y = m_fCameraHeight; //0.5f;
	//이걸 흔들면 쉐이킹 효과를 줄 수 있을 것이다
	m_vEye *= m_fDistance;	// 방향 벡터

	m_vEye += pPlayerTransform->m_vInfo[INFO_POS];
	m_vAt = pPlayerTransform->m_vInfo[INFO_POS];

	//
	//_matrix		matCamWorld;
	//D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);
	//_vec3		vRight;
	//memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

	//_matrix		matRot;
	//D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_fAngle));
	//D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	//m_vAt = m_vEye + vLook;



	// 카메라 보는 위치때메 더해줌
	m_vAt = m_vEye + vLook;
	//if (Get_DIKeyState(DIK_Q) & 0X80)
	//{
	//	_matrix		matCamWorld;
	//	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	//	//if (m_bExecution)
	//	{
	//	//::PlaySoundW(L"executionEffect.wav", SOUND_EFFECT, 0.1f);
	//	_vec3		vLook;
	//	memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

	//	_vec3		vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.f * 1;

	//	m_vEye -= vLength;
	//	m_vAt -= vLength;
	//	m_bExecution = false;
	//	}
	//}
	

}

void CMyCamera::Excution_Renewal(const _float & fTimeDelta)
{
	
	if (!m_bSoundCheck)
	{
		::PlaySoundW(L"executionEffect.wav", SOUND_EFFECT, 0.1f);
		m_bSoundCheck = true;
	}

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	//
	_vec3		vLook;
	memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

	_vec3		vLength = *D3DXVec3Normalize(&vLook, &vLook) * 1.5f * fTimeDelta;


	m_vEye -= vLength;
	m_vAt -= vLength;
	
	
	

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
