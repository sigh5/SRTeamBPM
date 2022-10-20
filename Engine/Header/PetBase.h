#pragma once
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CPetBase : public CGameObject
{
protected:
	explicit CPetBase(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPetBase();

public:
	virtual HRESULT		Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

	virtual void		Get_ToPlayerDistance(float* _Distance);

private:
	CDynamic_Transform*		m_pDynamicTransformCom = nullptr;
	_vec3			m_vPlayerPos;
	_vec3			m_vPetPos;

public:
	virtual void	Free(void);
};

END