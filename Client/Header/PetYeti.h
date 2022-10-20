#pragma once
#include "MonsterBase.h"
class CPetYeti :public CGameObject
{
private:
	explicit CPetYeti(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPetYeti();

public:
	virtual HRESULT		Ready_Object(float	Posx, float Posz);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

	void			Dance_Judge(const _float& fTimeDelta);
	void			Dance(const _float& fTimeDelta);
	void			  Get_MonsterToPlayer_Distance(float* _Distance);
public:
	static CPetYeti*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float Posx = 0, float Posz = 0);
	virtual void	Free(void);

private:
	CTransform*		m_pTransform = nullptr;
	CAnimation*		m_pAnimationCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pBubbleTextureCom = nullptr;
	CTexture*		m_pDanceTextureCom = nullptr;
	CAnimation*		m_pDanceAnimationCom = nullptr;

	float fMtoPDistance = 0.f;
	_vec3		m_vPlayerPos, m_vMonsterPos;
	bool		m_bDance = false;
	float		m_fDanceDelay = 0.f;
	float		m_fDanceTimeCount = 0.f;

	int			m_iPetPattern = 0;

	bool		m_bTalk = false;

	
	int			m_iState = 0;
	int			m_iDanceNum = 0;

	bool		m_bAnireset = false;
};

