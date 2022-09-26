#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)

//class CUserInterface;

class CHpBar : public CGameObject
{
private:
	explicit CHpBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHpBar();

public:
	virtual		HRESULT		Ready_Object(void)							override;
	virtual		_int		Update_Object(const _float& fTimeDelta)		override;
	virtual		void		LateUpdate_Object(void)						override;
	virtual		void		Render_Obejct(void)							override;

public:
	static CHpBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

END