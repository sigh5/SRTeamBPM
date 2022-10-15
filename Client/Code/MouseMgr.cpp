#include "stdafx.h"
#include "..\Header\MouseMgr.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr()
	
{
	
}

CMouseMgr::~CMouseMgr()
{
	Free();
}


void CMouseMgr::Mouse_Change(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* pFilepath)
{
	if (m_bMouse)
	{
		IDirect3DSurface9_Release(m_Surfcursor);
		IDirect3DTexture9_Release(m_pCursortex);
	}

	// Mouse Set _ manager에 생성하기 
	D3DXCreateTextureFromFile(pGraphicDev, pFilepath, &m_pCursortex);
	m_pCursortex->GetSurfaceLevel(0, &m_Surfcursor);
	pGraphicDev->SetCursorProperties(0, 0, m_Surfcursor);
	m_bMouse = true;
	// ~Mouse Set
}




void CMouseMgr::Free(void)
{
	if (m_bMouse == true)
	{
		IDirect3DSurface9_Release(m_Surfcursor);
		IDirect3DTexture9_Release(m_pCursortex);
	}

}
