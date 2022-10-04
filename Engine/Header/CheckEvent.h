#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCheckEvent :public CComponent
{
private:
	CCheckEvent();
	virtual ~CCheckEvent();

	// Inherited via CComponent
	virtual CComponent * Clone(void) override;
};


END