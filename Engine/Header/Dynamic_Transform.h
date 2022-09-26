#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CDynamic_Transform : public CComponent
{
private:
	explicit CDynamic_Transform();
	explicit CDynamic_Transform(const CDynamic_Transform& rhs);
	virtual ~CDynamic_Transform();

public:
	HRESULT				Ready_Dynamic_Transform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);
	
public:
	static CDynamic_Transform*		Create(void);
	virtual CComponent*				Clone(void);


private:
	virtual void					Free(void);

};

END