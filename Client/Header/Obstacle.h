#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTexture;
	class CRcTex;
	class CTransform;
	class CCalculator;
	class CCollider;
	class CAnimation;
}

enum OBSTACLE
{
	OBSTACLE_CRYSTAL, OBSTACLE_STREET_LAMP , OBJSTACLE_TREE ,OBSTACLE_WALL_LAMP, OBSTACLE_FIRE , OBJSTACLE_END
};
//주석지우셈

class CObstacle : public CGameObject
{
private:
	explicit CObstacle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObstacle();

public:
	HRESULT				InitSetting(_vec2* vMousPos, const wstring& LayerName, wstring* RoomName);
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event()override;

public: //For Tool
	void				MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수
	_bool				Set_SelectGizmo();

public:
	void				Set_Layer_Map_Name(const wstring& LayerName, wstring* RoomName){m_LayerName = LayerName; m_RoomName = *RoomName;	}

	void				Set_TextureCom();

	void				Set_Light_Anim_Obj();
	void				Set_Light_Normal_Obj();

public:


public:
	const _int&			Get_Option() { return m_iOption; }
	void				Set_Option(_int iType) { m_iOption = iType; }


	HRESULT				SetUp_Material();
	HRESULT				SetUp_NormalMaterial();

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;
private:
	wstring				m_LayerName = L"";
	wstring				m_RoomName = L"";
	_int				m_iOption = 0;					// 0: Collision 1: NonCollision 
	

	_float				m_fFrame = 0.f;
	_bool				m_bControlAnim = false;
	_bool				m_bRythmeObstacle = false;

public:
	static	CObstacle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

};

