#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int Update_Object(const _float& fTimeDelta)	override;
	virtual void LateUpdate_Object(void)					override;
	_matrix GetView() const { return m_matView; }
	_matrix GetProj() const { return m_matProj; }

public:		// UI 뷰 포트(스크린 좌표)를 따기 위해 투영 좌표를 따는 함수
	_matrix*		Get_ProjMatrix(_matrix* matProj) { return matProj = &m_matProj; }

	_matrix*		Get_ViewMatrix(_matrix* matView) { return matView = &m_matView; }

public:
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix		m_matView, m_matProj,m_matOrth;
	
protected:
	virtual void		Free(void);
};
END