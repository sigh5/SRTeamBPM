#include "Base_Effect.h"
#include "Engine_Include.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CAnimation;
}

class CSkillEffct :public CBase_Effect
{
private:
	explicit CSkillEffct(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkillEffct();


public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Obejct(void) override;


	virtual HRESULT		Set_Effect_INFO(_int iEffcet_Owner, _int iStartAnimNum, _int iAnimMax, _float fIntervel);

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

public:
	_float				m_fFrame = 0.f;


public:
	static CSkillEffct*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = _vec3{ 0.f,0.f,0.f });
	virtual void	Free(void);
};

