#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRcTex;

class ENGINE_DLL CWreckBase :public CGameObject
{
protected:
	explicit CWreckBase(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWreckBase();

public:
	virtual HRESULT		Ready_Object(_vec3 vPos, _vec3 vDir);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;



protected:
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;

	_vec3			m_vDir;
	_vec3			m_vUpDown;

	HRESULT				Add_Component(void);

public:
	virtual void Free(void);
};

END