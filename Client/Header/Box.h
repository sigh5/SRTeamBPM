#pragma once
#include "GameObject.h"
#include "Engine_Include.h"


BEGIN(Engine)

	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CAnimation;
	class CCollider;

END

class CBox : public CGameObject
{
private:
	explicit CBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBox();

public:
	HRESULT				Ready_Object(_uint iX, _uint iY);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

private:
	HRESULT				Add_Component(void);
	
public:
	HRESULT				Open_Event(CGameObject* pGameObject);

	void				Set_Open(_bool pOpen) { m_bBoxOpen = pOpen; }
	

public:
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();


private:
	CTransform*			m_pTransCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;	
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	CGameObject*		m_pPlayer;

public:
	_bool				m_bBoxOpen = true;  // 박스 개방

	_bool				m_bTextRender = false;
	
public:
	static CBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY);
	virtual void		Free(void);

};

 