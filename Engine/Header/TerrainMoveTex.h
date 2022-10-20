#pragma once
#include "VIBuffer.h"

BEGIN(Engine)


class ENGINE_DLL CTerrainMoveTex :public CVIBuffer
{
private:
	explicit CTerrainMoveTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainMoveTex(const CTerrainMoveTex& rhs);
	virtual ~CTerrainMoveTex();


public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
	_ulong		Get_VtxCntX(void) const { return m_iH.biWidth; }
	_ulong		Get_VtxCntZ(void) const { return m_iH.biHeight; }


public:
	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	
	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv,float fTimeDelta);

	
	virtual void Render_Buffer(void) override;

	const _vec3&		Get_Center() { return m_vCenter; }

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3*				m_pPos;
	_bool				m_bClone;

	_vec3				m_vCenter;

public:
	static CTerrainMoveTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	CComponent*	Clone(void);
	virtual void Free(void) override;


};

END