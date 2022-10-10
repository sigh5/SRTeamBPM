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

enum OBSTACLE_ANIM
{
	OBSTACLE_CRYSTAL,OBSTACLE_STREET_LAMP, OBJSTACLE_TREE ,OBSTACLE_END
};
//�ּ������

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
	void				MousePostoScreen(); // ���� ���콺 ����Ŭ���� ��ġ�� ť�긦 ������ִ� �Լ�
	_bool				Set_SelectGizmo();

public:
	void				Set_TextureCom();




public:
	void				Set_Layer_Map_Name(const wstring& LayerName, wstring* RoomName){m_LayerName = LayerName; m_RoomName = *RoomName;	}


public:
	const _int&			Get_Option() { return m_iOption; }
	void				Set_Option(_int iType) { m_iOption = iType; }



private:
	HRESULT				Add_Component(void);
	_bool				m_bControlAnim = false;

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
	_bool				m_bAnimationStart = false;
	_float				m_fFrame = 0.f;
	
public:
	static	CObstacle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

};

