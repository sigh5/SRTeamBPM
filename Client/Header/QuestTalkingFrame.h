#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;

END

class CQuestTalkingFrame :public CUI_Base
{
private:
	explicit CQuestTalkingFrame(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestTalkingFrame();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	void				Create_Quest();

public:
	void				Set_Active(_bool bEvent) { m_bActive = bEvent; }


private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	
	_matrix				m_ProjMatrix;
private:
	_vec3				m_vecScale;
	_bool				m_bActive = false;
	_tchar*				pString = L"";

	_int				m_iTalkNum = 0;
public:
	static CQuestTalkingFrame*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free(void);

};

