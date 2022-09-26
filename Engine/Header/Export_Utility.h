#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Include.h"
#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Transform.h"
#include "Calculator.h"
#include "Texture.h"
#include "Collider.h"
#include "CharacterInfo.h"
#include "MonsterBase.h"


#include "Animation.h"
#include "Camera.h"

#include "ProtoMgr.h"
#include "Management.h"
#include "Renderer.h"
#include "Pipeline.h"
#include "ObjectMgr.h"



#include "LightMgr.h"

BEGIN(Engine)

// Management


inline HRESULT	Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);
inline HRESULT		Set_Scene(CScene* pScene);
inline _int		Update_Scene(const _float& fTimeDelta);
inline void		LateUpdate_Scene(void);
inline void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
inline 	CComponent*				Get_Component(const _tchar* pLayerTag,
	const _tchar* pObjTag,
	const _tchar* pComponentTag,
	COMPONENTID eID);
inline CGameObject*				Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline HRESULT			Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pInstance);

// Test
//inline CScene*		Get_Scene();


// ProtoMgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*		Clone_Proto(const _tchar* pProtoTag);
inline HRESULT			Delete_Proto(const _tchar* pProtoTag);

// Renderer

inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup(void);

// lightMgr

inline HRESULT	Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);



// ObjectMgr
inline void				Collect_Obj(CGameObject* pObj);
inline CGameObject*		Reuse_Obj(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXVECTOR3& vPos);


// FileMgr



inline void			Release_Utility(void);

#include "Export_Utility.inl"

END
#endif 
