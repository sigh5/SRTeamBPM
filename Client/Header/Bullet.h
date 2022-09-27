#pragma once
#include "Engine_Include.h"
#include "GameObject.h"
#include "TestPlayer.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CCubeTex;
class CLayer;
class CScene;
class CManagement;
END

class CBullet : public CGameObject
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet();

public:
	HRESULT				Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCubeTex*			m_pCubeTexCom = nullptr;
	CLayer*				m_pLayerCom = nullptr;
	CScene*				m_pSceneCom = nullptr;
	CManagement*		m_pManagementCom = nullptr;

private:
	_float				m_fFrame = 0.f;
	_vec3				m_vPlayerLook;	// 플레이어 보는 방향

	// 추후에 카메라 좌표 따기 위한 변수
	//_vec3				m_vCamLook;

	// 3 Sec after, Bullet is Die.
	_float				m_fDeadTime;
	// ~3 Sec after, Bullet is Die.

public:
	static CBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
};

