#pragma once
#include "Engine_Include.h"
#include "UI_Base.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CEndingImage : public CUI_Base
{
private:
	explicit CEndingImage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEndingImage(const CUI_Base& rhs);
	virtual ~CEndingImage();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

	void						Set_Render(_bool _bRender) { m_bRender = _bRender; }
	_bool						Get_Render(void) { return m_bRender; }

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;

private:
	_vec3						m_vecScale;
	_bool						m_bRender = false;
	_float						m_fCount = 1200.f;

public:
	static CEndingImage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	virtual void Free(void);
};

