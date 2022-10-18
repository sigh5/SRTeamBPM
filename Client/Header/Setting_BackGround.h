#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CSetting_BackGround : public CUI_Base
{
private:
	explicit CSetting_BackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSetting_BackGround();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;

public:
	static CSetting_BackGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void Free(void);
};

