#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;
class CCollider;
END

class CWallCube : public CGameObject
{
private:
	explicit			CWallCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CWallCube();
public:
	HRESULT				InitSetting(_vec2* vMousPos, const wstring& LayerName, wstring* RoomName);
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Obejct(void) override;
	virtual void		Collision_Event() override;

public:// For Tool
	void				MousePostoScreen(); // 현재 마우스 더블클릭한 위치로 큐브를 만들어주는 함수
	_bool				Set_SelectGizmo();
	_bool*				Get_WireFrame() { return &m_bWireFrame; }
	void				Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }
	void				Set_Layer_Map_Name(const wstring& LayerName, wstring* RoomName)
	{m_LayerName = LayerName;
	m_RoomName = *RoomName;	}
	const _int&			Get_Option() { return m_iOption; }
	void				Set_Option(CUBE_TYPE eType) { m_iOption = (_int)eType; }
	// ~For Tool
private:
	HRESULT				Add_Component(void);

public:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	_bool				m_bIsOnterrrain = false;
	_int				m_iOption = 0;					// 0: Wall 1: Obstacle 2: TeleportStart 3: TelePortEnd

	wstring				m_LayerName =L"";

public:
	wstring				m_RoomName =L"";
	_int				irandNum=4;					// Room이 6개니까
	_int				iIndex = 0;
	

public:
	static CWallCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual void		Free()override;

};



