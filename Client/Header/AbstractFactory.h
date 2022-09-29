#pragma once

#include "GameObject.h"
#include "Component.h"

#include "Engine_Include.h"

template <typename T>
class CAbstractFactory
{

public:
	// Only Component
	static T* Clone_Proto_Component(const _tchar *protoName, map<const _tchar*, CComponent*>* mapComponent, COMPONENTID eID)
	{
		T* pComponent = dynamic_cast<T*>(::Clone_Proto(protoName));
		NULL_CHECK_RETURN(pComponent, nullptr);
		mapComponent[eID].insert({ protoName,pComponent });
		return pComponent;
	}











public:
	CAbstractFactory() {}
	~CAbstractFactory(){}
};

