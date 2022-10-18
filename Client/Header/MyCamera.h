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

	void		Excution_Renewal(const _float& fTimeDelta);


private:
	_bool			m_bFix = false;
	_bool			m_bCheck = true;
public:
	_int			Get_BillBoardDir() { return m_iBillBoardDir; }
	void			Set_CamDistance(_float fDistance) {m_fDistance = fDistance;}
	void			Set_Excution(_bool bExcution) { m_bExecution = bExcution; }
	void			Set_PlayerHit(_bool bHitCameraCam) { m_bPlayerHit = bHitCameraCam; }


public: 
	const _bool&	Get_InventroyActive() { return m_bInventroyActive; }
	void			Set_inventroyActive(_bool  bInventroyActive) { m_bInventroyActive = bInventroyActive; }

	void			Set_PlayerDeadCam(_bool bEvent) { m_bPlayerDead = bEvent; }
	_float			m_fOriginAngle = 0.f;
public:
	const _bool&   Get_InventroyActive() { return m_bInventroyActive; }
	void         Set_inventroyActive(_bool  bInventroyActive) { m_bInventroyActive = bInventroyActive; }

	void			Set_ShakeCheck(_bool bShake) { m_bEarthShake = bShake; }

private:
	_bool			m_bExecution = false;
	_bool			m_bSoundCheck = false;
	_bool			m_bPlayerHit = false;

private:
	_float			m_fDistance = 1.f;
	_float			m_fSpeed = 5.f;
	_float			m_fAngle = 0.f;

	_float			m_fCameraHeight = 0.5f;
	_float			m_fCameraOriginHeight = 0.5f;
	
	_int			m_itemp = 1;
	_float			m_fOriginFov = 0.f;

	_int			m_iBillBoardDir = 0;	// 0 왼쪽 1 오른쪽

	_float			m_fFrame = 0.f;
	_bool			m_bInventroyActive = false;

	_bool			m_bPlayerDead = false;
	_float			m_fDeadTimer=0.f;
	
	_bool			m_bEarthShake = false;
	_float			m_fShakeTimer = 0.f;
	// 추가
	_bool         m_bInventroyActive = false;


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

