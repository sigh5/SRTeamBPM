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

class CAnubis :public CMonsterBase
{
private:
	explicit CAnubis(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnubis();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Set_Pos(_vec3 vPos);
	//bool				Set_SelectGizmo();
	//MonsterInfo*		Get_Info(void) { return &m_Info; }
//private:
//	HRESULT				Add_Component(void);
//	void				Set_OnTerrain(void);
//	bool				Set_TransformPositon();


private:


	_float				m_fTimeDelta = 0.f;


	int					m_iPreIndex = 0; //반복적 재할당 방지용 변수


public:
	static CAnubis*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void	Free(void);
};

