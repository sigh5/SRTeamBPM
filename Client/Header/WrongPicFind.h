#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CWrongPicFind : public CUI_Base
{
private:
	explicit CWrongPicFind(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWrongPicFind(const CUI_Base& rhs);
	virtual ~CWrongPicFind();

public:
	virtual HRESULT				 Ready_Object() override;
	virtual _int				 Update_Object(const _float& fTimeDelta) override;
	virtual void				 LateUpdate_Object(void) override;
	virtual void				 Render_Obejct(void) override;

	void						 Picking_WrongPoint(void);

	_bool						 Get_Success(void) { return m_bFinalSuccess; }
private:
	HRESULT						 Add_Component(void);

private:
	CRcTex*					     m_pBufferCom = nullptr;
	CTransform*					 m_pTransCom = nullptr;
	CTexture*					 m_pTextureCom = nullptr;

private:
	_matrix						m_ProjMatrix;
	RECT						m_rcFindWrong[3];

	_bool						m_bSuccess0 = false;  
	_bool						m_bSuccess1 = false;
	_bool						m_bSuccess2 = false;
	
	_bool						m_bFinalSuccess = false;

public:
	static CWrongPicFind*		 Create(LPDIRECT3DDEVICE9 pGrhaphicDev);
		
private:	
	virtual void			     Free(void);
};

