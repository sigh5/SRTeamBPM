#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"
// 여기에서 폰트까지 관리하고 

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;
END

class CUI_Timer :public CUI_Base
{
private:
	explicit CUI_Timer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI_Timer();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	void				Time_Over();

public:
	void				Set_Active(_bool bEvent) { m_bActive = bEvent; }

	const _float&		Get_Time() { return m_fFrame; }

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

private:
	_matrix				m_ProjMatrix;
	_vec3				m_vecScale;
	_bool				m_bActive = false;
	wstring				pString = L"남은시간";
	wstring				pString2 = L"남은시간";
	wstring				pString3 = L"남은시간";
	_int				m_iTalkNum = 0;

	_float				m_fFrame = 0.f;
	_bool				m_bClearcheck = false;

	_bool				m_bFontCheck = false;


public:
	static CUI_Timer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free(void);

};

