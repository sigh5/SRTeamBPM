#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CCamera;
class CTransform;

class ENGINE_DLL CUserInterface : public CGameObject
{
protected:
	explicit CUserInterface(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUserInterface();

public:
	virtual		HRESULT		Ready_Object(void)							override;
	virtual		_int		Update_Object(const _float& fTimeDelta)		override;
	virtual		void		LateUpdate_Object(void)						override;
	virtual		void		Render_Obejct(void)							override;


public:
	void					UpdateOrthogonal(_float fTextureSizeX, _float fTextureSizeY);
	void					CalculateOrtho();


private:
	CCamera*		m_pCameraCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

protected:
	_vec3		m_vPos;
	
	//_matrix		m_matWorld, m_matView, m_matProj, m_matViewPortUI;
	
public:
	virtual void	Free(void);
};

END