#pragma once

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

enum BULLET_ID
{
	MONSTER_BULLET,
	PLAYER_BULLET,



	BULLET_ID_END,
};



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


public:
	void	Set_MoveDir(const wstring& LayerName, const wstring& GameObjectName, const wstring& ComponentName, COMPONENTID eID, _vec3 * vPos, BULLET_ID eBulletID, _vec3* vScale);


protected:
	_float				m_fFrame = 0.f;
	_vec3				m_MoveDir;
	_vec3				m_vScale;
	
protected:
	virtual void		Free(void);

};


END