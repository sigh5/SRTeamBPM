#pragma once
#include "UI_Base.h"
#include "Engine_Include.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class COrthoTransform;
class CCalculator;
class CAnimation;

class CCollider;

class CEquipmentBase;
class CGameObject;

END

typedef struct TagItemSlot
{
	_vec2	ItemSlotSize[4][9];	// 슬롯 갯수 및 몇 번 슬롯인지를 알기 위한 변수

	CGameObject* pItem[4][9];			// 실질적인 담긴 아이템 정보
	
}ItemSlot;

class CInventory_UI : public CUI_Base
{
private:
	explicit CInventory_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory_UI(const CUI_Base& rhs);
	virtual ~CInventory_UI();

public:
	HRESULT				Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta) override;

	virtual	void		LateUpdate_Object(void);
	virtual void		Render_Obejct(void) override;

	void				Find_Equip_Item(void);

	_bool				Get_InvenSwitch(void) { return m_bInvenSwitch; }

	ItemSlot*			Get_ItemSlot(void) { return &m_SlotType; }


	_uint				Get_MaxRow(void) { return m_iMaxRow; }
	_uint				Get_MaxColumn(void) { return m_iMaxColumn; }

private:
	HRESULT				Add_Component(void);
	
public:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	COrthoTransform*	m_pTransCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CAnimation*			m_pAnimationCom = nullptr;

	CCollider*			m_pColliderCom = nullptr;

private:
	_vec3						m_vecScale;

	_bool						m_bInvenSwitch = false;	
	vector<CGameObject*>		m_vecWeaponType;

	// 인벤토리 내 아이템 피킹 및 칸에 들어가기 위한 변수

	// 2중 for문 용
	_uint		m_iMaxRow = 9; // '행'. 가로 칸 갯수
	_uint		m_iMaxColumn = 4; // '열'. 세로 칸 갯수

	// 슬롯 1칸의 X, Y
	_float		m_fSlotX = 0.f;
	_float		m_fSlotY = 0.f;

	ItemSlot	m_SlotType;
	stack<CGameObject*>		m_stackWeapon;

public:
	vector<CGameObject*>*	Get_WeaponType(void) { return &m_vecWeaponType; }
	stack<CGameObject*>*	Get_Weapon(void) { return &m_stackWeapon; }
public:
	static CInventory_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free();
};

