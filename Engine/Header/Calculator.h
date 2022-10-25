#pragma once
#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CCubeTex;
class CRcTex;
class CCubeNormalTex;
class CEquipmentBase;
class CTerrainMoveTex;

class ENGINE_DLL CCalculator :	public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator(void);

	_float		HeightOnTerrain(_vec3* pPos, 
								const _vec3* pTerrainVtxPos, 
								const _ulong& dwCntX, 
								const _ulong& dwCntZ,
								const _ulong& dwVtxItv = 1);

	_vec3		PickingOnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom);

	//Cal cubePositon
	_vec3		PickingOnTerrainCube(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom);
	// Cal CubeRayHit
	_bool	PickingOnTransform(HWND hWnd, const CCubeTex* pCubeTexBufferCom, const CTransform* pCubeTransCom);

	_bool	PickingOnTransform(HWND hWnd, const CCubeNormalTex* pCubeTexBufferCom, const CTransform* pCubeTransCom);

	_bool	PickingOnTransform_Monster(HWND hWnd, const CRcTex* pMonsterTexBuffer, const CTransform* pMonsterTransCom);
	
	_bool	PickingTerrainObject(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom);

	_bool	PickingOnTransform_RcTex(HWND hWnd, const CRcTex* pRcTexCom, const CTransform* pTransform);

	_bool	Picking_OrthoUI(HWND hWnd, const CRcTex* pRcTexCom, const CTransform* pUITransform);

	POINT*	Get_MouseAxis(void) { return &m_ptMouse; }

	_vec3*	Get_MouseProjAxis(HWND hWnd);

private:
	POINT		m_ptMouse{};
	_vec3		m_vecProjMouse;

public:
	virtual CComponent* Clone(void) override;
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

END