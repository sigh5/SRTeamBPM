#pragma once
#include "GameObject.h"
namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}
class CMonsterToolObject :
	public CGameObject
{
private:
	explicit CMonsterToolObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterToolObject(const CMonsterToolObject& rhs);
	virtual ~CMonsterToolObject();

public:
	virtual HRESULT		Ready_Object(int Posx, int Posy);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	bool				 Set_SelectGizmo(HWND g_hWn, CCalculator* _pCalcul, CRcTex* _pBuffer);
	int&				 Get_MonsterType() { return m_iMonsterIndex; }
	CCalculator*		Get_Calculator(void) { return m_pCalculatorCom; }
	CRcTex*				Get_Buffer(void) { return m_pBufferCom; }
	bool Set_TransformPositon(HWND g_hWnd, CCalculator* _pCalcul);
private:
	CTexture*	m_pTextureCom = nullptr;
	CRcTex*		m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;
	CCalculator* m_pCalculatorCom = nullptr;
	int m_iMonsterIndex = 0;
	int m_iPreIndex = 0;
public:
	static CMonsterToolObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx = 0, int Posy = 0);
	virtual void		Free(void);
};

