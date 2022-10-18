#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CSoundBox_UI : public CUI_Base
{
private:
	explicit CSoundBox_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSoundBox_UI(const CUI_Base& rhs);
	virtual ~CSoundBox_UI();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

	_uint		Get_Volume(void) { return m_iVolume; }

	void		Set_Volume(_uint _iVolume) { m_iVolume = _iVolume; }

private:
	HRESULT		Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

private:
	_uint		m_iVolume = 0;

	_uint		m_iMaxVol; 
	_uint		m_iMinVol; 

	wstring		m_szVolume = L"";

	_bool		m_bPlusSound = false;
	_bool		m_bMinusSound = false;

public:
	static CSoundBox_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY);

private:
	virtual		void Free(void);
};

