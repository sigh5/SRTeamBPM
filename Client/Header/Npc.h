#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CAnimation;
	class CCharacterInfo;
	class CCollider;
}

class CNpc :public CGameObject
{
private:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpc();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Collision_Event();

public:
	const _bool&	Get_ShopUICheck() { return m_fShopUICheck; }
	void			Set_ShopUICheck(_bool bShopUICheck) { m_fShopUICheck= bShopUICheck; }

private:
	HRESULT				Add_Component(void);
public:
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();


private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;


	_bool				m_fShopUICheck = false;

public:
	static CNpc*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);


};

