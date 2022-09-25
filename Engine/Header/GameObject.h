#pragma once

#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject :	public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*				Get_Component(const _tchar* pComponentTag, COMPONENTID eID);

public:
	void		Set_DrawTexIndex(_int iTexIndex) { m_iTexIndex = iTexIndex; }
	const _int	Get_DrawTexIndex()const {return m_iTexIndex;}

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Obejct(void);
	virtual		void		Set_Pos(_vec3 vPos) {}
private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];

protected:
	_int				m_iTexIndex = 0;
public:
	static CGameObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

public:
	virtual void	Free(void);
};

END