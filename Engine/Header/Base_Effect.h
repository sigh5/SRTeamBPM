#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)


enum  OBJ_OWNER
{
	OWNER_PALYER, OWNER_ANUBIS, OWNER_GHUL, OWNER_OBELISK, OWNER_SPIDER, OWNER_FATBAT, OWNER_EARTHSHAKER, OWNER_SPHINX,							OWNER_END
};


typedef struct INFO_Tag_Effect
{
	OBJ_OWNER iEffcet_Owner;		// ex) Player, Monster, Bullet, Obstacle , Wall ... 
	_int iStartAnimNum;		// Start Anim Num
	_int iAnimMax;			// if you have 8 Write 7
	_float fIntervel;			// intervel 
	_float fFrameTime;		// How Long Draw

}INFO_Effect;





class ENGINE_DLL CBase_Effect :public CGameObject
{
protected:
	explicit CBase_Effect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBase_Effect(const CBase_Effect& rhs);
	virtual ~CBase_Effect();

public:
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);

	
public: // 공통기능
	virtual HRESULT		Set_Effect_INFO(_int iEffcet_Owner,
								_int iStartAnimNum,
								_int iAnimMax, _float fIntervel);



protected:
	INFO_Effect			m_eEffect;
	_matrix				m_matEffectProj;
	_float				m_fFrame;
	
public:
	virtual void	Free(void);


};


END