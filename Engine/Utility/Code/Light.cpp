#include "..\..\Header\Light.h"

#include "Export_Function.h"

USING(Engine)

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_iIndex(0)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	

	m_pGraphicDev->SetLight(iIndex, &m_tLightInfo);
	m_pGraphicDev->LightEnable(iIndex, TRUE);

	return S_OK;
}


CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*		pInstance = new CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
	{
		MSG_BOX("CLight Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLight::Free(void)
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);
	Safe_Release(m_pGraphicDev);
}
