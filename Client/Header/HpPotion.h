#pragma once

#include "GameObject.h"


namespace Engine
{
	class CTransform;
	class CRcTex;
	class CCalculator;
	class CTexture;
	class CAnimation;
}
class CHpPotion :public CGameObject
{
private:
	explicit CHpPotion(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHpPotion();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int	Update_Object(const _float& fTimeDelta);
	virtual void	LateUpdate_Object(void);
	virtual void	Render_Obejct(void);
	virtual void	Set_Pos(_vec3 vPos);
	//플레이어와 충돌 시 실행할 함수를 추가해야 함

private:
	HRESULT				Add_Component(void);
	void				Set_OnTerrain(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

public:
	static CHpPotion*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void	Free(void);
};
