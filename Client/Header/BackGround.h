#pragma once

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CShader;

END

class CBackGround :	public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;


private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

private:
	_float				m_fFrame = 0.f;
	_vec3				m_vOriginScale;


	_int				m_iTempNum = 0;
	


public:
	static CBackGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	virtual void Free(void);


};

