#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CThunderTex :
	public CVIBuffer
{
private:
	explicit CThunderTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CThunderTex(const CThunderTex& rhs);
	virtual ~CThunderTex();

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }



private:
	_vec3*				m_pPos;
	_bool				m_bClone;

public:
	static CThunderTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*	Clone(void);
	virtual void Free(void) override;
};

END