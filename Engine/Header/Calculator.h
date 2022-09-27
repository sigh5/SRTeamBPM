#pragma once
#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CCubeTex;
class CRcTex;

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

	_bool	PickingOnTransform_Monster(HWND hWnd, const CRcTex* pMonsterTexBuffer, const CTransform* pMonsterTransCom);

public:
	virtual CComponent* Clone(void) override;
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

END