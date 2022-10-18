#pragma once
#include "WreckBase.h"
class CObeliskWreck :public CWreckBase
{
private:
	explicit CObeliskWreck(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObeliskWreck();

public:
	HRESULT				Ready_Object(_vec3 vPos, _vec3 vDir) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

public:
	static CObeliskWreck*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRight);
	virtual void		Free(void);

private:
	int	 m_iTextureNum = 0;
};

