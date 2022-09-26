#pragma once
#include "Transform.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CCalculator;

class ENGINE_DLL CDynamic_Transform : public CTransform
{
private:
	explicit CDynamic_Transform();
	explicit CDynamic_Transform(const CDynamic_Transform& rhs);
	virtual ~CDynamic_Transform();

public:
	HRESULT				Ready_Dynamic_Transform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	
	_float				Get_TerrainY1(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	void				Jumping(_float _JumpPower, _float _TimeDelta);
	
public:
	static CDynamic_Transform*		Create(void);
	virtual CComponent*				Clone(void);

	CCalculator*					m_pCalculator = nullptr;


private:
	virtual void					Free(void);

};

END