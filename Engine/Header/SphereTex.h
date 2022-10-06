#pragma once
#include "VIBuffer.h"

BEGIN(Engine)


class ENGINE_DLL CSphereTex :public CVIBuffer
{
public:
	explicit CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereTex(const CSphereTex& rhs);
	virtual ~CSphereTex();


public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	static CSphereTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

	// Inherited via CVIBuffer
	virtual CComponent * Clone(void) override;
	virtual void Free(void) override;

private:
	LPD3DXMESH	m_pMesh = nullptr;

	LPDIRECT3DINDEXBUFFER9 m_pIndexVB = nullptr;
	

};

END