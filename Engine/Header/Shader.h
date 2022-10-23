#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)


class ENGINE_DLL CShader :public CComponent
{
public:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

public:
	HRESULT				Ready_Shader(const _tchar* pShaderFilePath);
	HRESULT				Begin_Shader(_uint iPassIndex);
	HRESULT				End_Shader();


public:
	HRESULT				Set_Raw_Value(D3DXHANDLE hHandle, void *pData, _uint iLength);
	HRESULT				Set_Texture(D3DXHANDLE hHandle, IDirect3DBaseTexture9* pTexture);
	HRESULT				Set_Bool(D3DXHANDLE hHandle, const _bool& bEvent);
	HRESULT				Set_ArrayRange(D3DXHANDLE hHandle, int* pData, _uint iLength);


public:
	static CShader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pShaderFilePath);
	virtual CComponent*	Clone(void);

private:
	LPD3DXEFFECT		m_pEffect = nullptr;


protected:
	virtual void		Free(void);
};

END