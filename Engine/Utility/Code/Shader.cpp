#include "..\..\Header\Shader.h"

#include "Export_Function.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CShader::CShader(const CShader & rhs)
	:CComponent(rhs) ,m_pEffect(rhs.m_pEffect)
{
	m_pEffect->AddRef();
}

CShader::~CShader()
{
}

HRESULT CShader::Ready_Shader(const _tchar * pShaderFilePath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pShaderFilePath, nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Begin_Shader(_uint iPassIndex)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(iPassIndex);

	return S_OK;
}

HRESULT CShader::End_Shader()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->EndPass();
	m_pEffect->End();
	
	return S_OK;
}

HRESULT CShader::Set_Raw_Value(D3DXHANDLE hHandle, void * pData, _uint iLength)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	return m_pEffect->SetRawValue(hHandle, pData, 0, iLength);

}

HRESULT CShader::Set_Texture(D3DXHANDLE hHandle, IDirect3DBaseTexture9 * pTexture)
{
	if (m_pEffect == nullptr)
		return E_FAIL;


	return m_pEffect->SetTexture(hHandle, pTexture);
}

HRESULT CShader::Set_Bool(D3DXHANDLE hHandle, const _bool & bEvent)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	return m_pEffect->SetBool(hHandle,bEvent);
}

HRESULT CShader::Set_ArrayRange(D3DXHANDLE hHandle, int* pData, _uint iLength)
{

	if (m_pEffect == nullptr)
		return E_FAIL;

	m_pEffect->SetIntArray(hHandle, pData, iLength);


	return E_NOTIMPL;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pShaderFilePath)
{
	CShader* pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pShaderFilePath)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CShader::Clone(void)
{
	return new CShader(*this);
}

void CShader::Free(void)
{
	CComponent::Free();

	Safe_Release(m_pEffect);
}
