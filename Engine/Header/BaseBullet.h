#pragma once

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)


class ENGINE_DLL CBaseBullet :public CGameObject
{
protected:
	explicit CBaseBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBaseBullet(const CBaseBullet& rhs);
	virtual ~CBaseBullet();

public:
	virtual HRESULT		Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

protected:
	_float				m_fFrame = 0.f;
	// 3 Sec after, Bullet is Die.
	_float				m_fDeadTime;
	// ~3 Sec after, Bullet is Die.
	_vec3				m_MoveDir;
	

protected:
	virtual void		Free(void);

};


END