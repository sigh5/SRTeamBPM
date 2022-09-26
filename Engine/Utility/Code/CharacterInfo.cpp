#include "..\..\Header\CharacterInfo.h"

#include "Export_Function.h"

USING(Engine)


CCharacterInfo::CCharacterInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_bClone(false)
{
}


CCharacterInfo::CCharacterInfo(const CCharacterInfo & rhs)
	: CComponent(rhs), m_bClone(true)
{
}

CCharacterInfo::~CCharacterInfo()
{
}

HRESULT CCharacterInfo::Ready_CharacterInfo()
{
	return E_NOTIMPL;
}

CCharacterInfo * CCharacterInfo::Clone(void)
{
	return new CCharacterInfo(*this);
}

CCharacterInfo * CCharacterInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCharacterInfo*	pInstance = new CCharacterInfo(pGraphicDev);

	if (FAILED(pInstance->Ready_CharacterInfo()))
	{
		MSG_BOX("Collider Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
