#pragma once
#include "Transform.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CCalculator;

class ENGINE_DLL CDynamic_Transform : public CTransform
{
private:
	explicit CDynamic_Transform(void);
	explicit CDynamic_Transform(const CDynamic_Transform& rhs);
	virtual ~CDynamic_Transform();

public:
	HRESULT				Ready_Dynamic_Transform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	
	_float				Get_TerrainY1(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID, CCalculator* pCalculator, CTransform* pTransform);
													//L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC, m_pCalculatorCom, m_pTransCom); 
	void				Jumping(_float _JumpPower, _float _TimeDelta, CTransform* pTransform);
	void				Monster_Fly(CTransform* _pTransformCom, float _TerrainY, float _MaxY);

	void				Dashing(const _float& fTimeDelta, CTransform* pTransform, _vec3 vecUp, _vec3 vecDir, TYPING_TYPE tType);

public:
	static CDynamic_Transform*		Create(void);
	virtual CComponent*				Clone(void);

	//CCalculator*					m_pCalculator = nullptr;

private:
	_bool				m_bClone;

private:
	virtual void					Free(void);

};

END

