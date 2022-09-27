#pragma once
#include "Transform.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL COrthoTransform :public CTransform
{
private:
	explicit COrthoTransform(void);
	explicit COrthoTransform(const COrthoTransform& rhs);
	virtual ~COrthoTransform();

public:
	HRESULT				Ready_OrthoTransform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	void				OrthoMatrix(_float fScaleX,
									_float fScaleY,
									_float fPosX,
									_float fPoxY , 
									_float fWidth, _float fHeight);




public:
	_matrix					m_matView;
	_matrix					m_matOrtho;


public:
	static COrthoTransform*		Create(void);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);
};

END