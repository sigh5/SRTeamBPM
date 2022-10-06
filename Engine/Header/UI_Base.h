#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCharacterInfo;
class CTransform;
class CRcTex;
class CTexture;
class CAnimation;
class CCalculator;
class CDynamic_Transform;
class COrthoTransform;


class ENGINE_DLL CUI_Base : public CGameObject
{
protected:
	explicit CUI_Base(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Base(const CUI_Base& rhs);
	virtual ~CUI_Base();

public:
	virtual HRESULT		 Ready_Object(void)							override;
	virtual _int		 Update_Object(const _float& fTimeDelta)	override;
	virtual void		 LateUpdate_Object(void)					override;

	void				Set_OrthoMatrix(_float fSizeX,_float fSizeY,_float fPosX,_float fPosY);

	void				Get_ProjMatrix(_matrix* pWorld) const { *pWorld = m_matProjMatrix; }
protected:
	CCharacterInfo*		m_pInfoCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CDynamic_Transform* m_pDynamicTransCom = nullptr;
	COrthoTransform*	m_pOrthoTransCom = nullptr;

protected:
	virtual HRESULT				Add_Component(void);

protected:
	_float	m_fX = 0;
	_float  m_fY = 0;
	_float  m_fSizeX = 0.f;
	_float  m_fSizeY = 0.f;

	_matrix	m_matProjMatrix;

public:
	virtual void		Free(void);
};

END