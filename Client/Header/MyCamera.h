#pragma once
#include "Camera.h"
#include "Engine_Include.h"

class CMyCamera : public CCamera
{
private:
	explicit CMyCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMyCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_Object(const _float& fTimeDelta) override;

	virtual void LateUpdate_Object(void) override;
	void		Mouse_Move(const _float& fTimeDelta);
	void		Mouse_Fix(void);


private:
	void		Key_Input(const _float& fTimeDelta);
	void		Target_Renewal(const _float& fTimeDelta);


private:
	_bool			m_bFix = false;
	_bool			m_bCheck = true;
public:
	_int			Get_BillBoardDir() { return m_iBillBoardDir; }

	void			Set_CamDistance(_float fDistance) {m_fDistance = fDistance;}

public:
	_bool			m_bExecution = false;


private:
	_float			m_fDistance = 1.f;
	_float			m_fSpeed = 5.f;
	_float			m_fAngle = 0.f;

	_float			m_fCameraHeight = 0.5f;
	_int			m_iBillBoardDir = 0;	// 0 ¿ÞÂÊ 1 ¿À¸¥ÂÊ

public:
	static CMyCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void Free(void) override;


};

