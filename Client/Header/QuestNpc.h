#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CCollider;
	class CAnimation;
	class CLayer;
}

class CQuestNpc :public CGameObject
{
private:
	explicit CQuestNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuestNpc();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;
	virtual void	Collision_Event();


private:
	HRESULT				SetUp_Material(void);
	void				Set_Light_Obj();
	bool				Quest_clear(CLayer* pLayer);

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;

	CCollider*			m_pColliderCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	_bool				m_bPlayerInterection = false;




public:
	static CQuestNpc*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

};

