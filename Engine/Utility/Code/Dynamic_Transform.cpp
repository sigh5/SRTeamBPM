#include "..\..\Header\Dynamic_Transform.h"
#include "Export_Function.h"

USING(Engine)

CDynamic_Transform::CDynamic_Transform()
{
}

CDynamic_Transform::CDynamic_Transform(const CDynamic_Transform & rhs)
	: CComponent(rhs)
{
}


CDynamic_Transform::~CDynamic_Transform()
{
}

HRESULT CDynamic_Transform::Ready_Dynamic_Transform(void)
{
	return S_OK;
}

_int CDynamic_Transform::Update_Component(const _float & fTimeDelta)
{
	CComponent::Update_Component(fTimeDelta);

	return 0;
}

CDynamic_Transform * CDynamic_Transform::Create(void)
{
	CDynamic_Transform* pInstance = new CDynamic_Transform;
	
	if (FAILED(pInstance->Ready_Dynamic_Transform()))
	{
		Safe_Release(pInstance);
		
		return nullptr;
	}
	return pInstance;
}

CComponent * CDynamic_Transform::Clone(void)
{
	return new CDynamic_Transform(*this);
}

void CDynamic_Transform::Free(void)
{
	CComponent::Free();
}
