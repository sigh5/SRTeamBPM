#pragma once

#include "BaseBullet.h"


BEGIN(Engine)

class CTransform;
class CTexture;
class CCubeTex;

END

class CBullet : public CBaseBullet
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet();

public:
	HRESULT				Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCubeTex*			m_pCubeTexCom = nullptr;

public:
	static CBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
};

