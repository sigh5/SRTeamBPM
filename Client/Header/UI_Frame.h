#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class COrthoTransform;
class CCalculator;

END


class CUI_Frame : public CUI_Base
{
private:
	explicit CUI_Frame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Frame(const CUI_Base& rhs);
	virtual ~CUI_Frame();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

public:
	_int				Get_ItemIndex() { return m_iItemIndex; }
	void				Set_ItemIndex(_int iNum) { m_iItemIndex += iNum; }



private:
	HRESULT				Add_Component(void);

public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

private:
	_vec3				m_vecScale;

	// 아이템 키값을 위한 변수
	_int				m_iItemIndex = 0;


public:
	static CUI_Frame*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free(void);
};

