#pragma once
#include "MonsterBase.h"


class CAnubis :public CMonsterBase
{
private:
	explicit CAnubis(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubis();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

public:
	static CAnubis*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);

private:
	int m_iPreIndex = 0;
};

