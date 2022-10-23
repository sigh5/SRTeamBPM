#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;

END

enum Quest_Index
{
	Quest_Index_ONE, Quest_Index_TWO, Quest_Index_THREE, Quest_Index_END

};


class CQuestProcessing_UI :public CUI_Base
{
private:
	explicit CQuestProcessing_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestProcessing_UI();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

public:
	void				Set_Quest_Claer(Quest_Index eIndex, _bool bEvent){m_bMini_Game_Clear_Check[eIndex] = bEvent;}
	_bool		MiniGameCheck();
private:
	HRESULT				Add_Component(void);

private:
	void				Current_QuestString();
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

private:
	_bool				m_bMini_Game_Clear_Check[Quest_Index_END] = { false, false,false };
	_bool				m_bQuestClear = false;
	_float				m_fFrame = 0.f;
	

public:
	static CQuestProcessing_UI*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free(void);

};

