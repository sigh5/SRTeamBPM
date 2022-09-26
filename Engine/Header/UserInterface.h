#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CCamera;
class CTransform;

class ENGINE_DLL CUserInterface : public CComponent
{
protected:
	explicit CUserInterface();
	explicit CUserInterface(const CUserInterface& rhs);
	virtual ~CUserInterface();

public:
	HRESULT					Ready_UserInterface(void);

	virtual _int			Update_Component(const _float& fTimeDelta);
	virtual void			LateUpdate_Component(void)	override;

	_matrix*				UpdateOrthogonal(_float fTextureSizeX, _float fTextureSizeY, _matrix* pMatrix);

private:
	CCamera*		m_pCameraCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

protected:
	_vec3		m_vPos;
	
	//_matrix		m_matWorld, m_matView, m_matProj, m_matViewPortUI;

private:
	static CUserInterface*		Create(void);
	
public:
	virtual CComponent*	Clone(void);
	virtual void	Free(void);
};

END