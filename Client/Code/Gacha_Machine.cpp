#include "stdafx.h"
#include "..\Header\Gacha_Machine.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "MyCamera.h"
#include "Player.h"

// 슬롯 돌려서 나올 수 있는 것
#include "Coin.h"
#include "HealthPotion.h"
#include "Key.h"
#include "FatBat.h"
#include "Spider.h"
#include "UI_Frame.h"



CGacha_Machine::CGacha_Machine(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGacha_Machine::~CGacha_Machine()
{
}

HRESULT CGacha_Machine::Ready_Object(_uint iX, _uint iY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos((_float)iX, 1.f, (_float)iY);
	m_pTransCom->Compulsion_Update();
	m_pAnimationCom->Ready_Animation(2, 0, 0.2f, 2);

	return S_OK;
}

_int CGacha_Machine::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	
	// m_pAnimationCom-> 플레이어가 소유한 코인을 1개 받으면 애니메이션이 돌다.

	CDynamic_Transform* pPlayerTrans = dynamic_cast<CDynamic_Transform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	
	_vec3	vecPlayerPos, vecCoinSlotPos;

	pPlayerTrans->Get_Info(INFO_POS, &vecPlayerPos);

	m_pTransCom->Get_Info(INFO_POS, &vecCoinSlotPos);

	_vec3	vecDir = vecPlayerPos - vecCoinSlotPos;

	_float	fDistance = D3DXVec3Length(&vecDir);

	if (fDistance <= 4.f)
		m_bTextRender = true;

	else
		m_bTextRender = false;


	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CGacha_Machine::LateUpdate_Object(void)
{
	CMyCamera* pCamera = static_cast<CMyCamera*>(Get_GameObject(L"Layer_Environment", L"CMyCamera"));
	NULL_CHECK(pCamera);
 
	_matrix		matWorld, matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	memcpy(&matBill, &matView, sizeof(_matrix));
	memset(&matBill._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix      matScale, matTrans;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);

	_matrix      matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, (_float)pCamera->Get_BillBoardDir());

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y,
		vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale* matRot * matBill * matTrans;
	m_pTransCom->Set_WorldMatrix(&(matWorld));
	

	Engine::CGameObject::LateUpdate_Object();
}

void CGacha_Machine::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->LightEnable(0, FALSE);
	m_pGraphicDev->LightEnable(1, FALSE);
	m_pGraphicDev->LightEnable(2, FALSE);
	m_pGraphicDev->LightEnable(3, FALSE);
	m_pGraphicDev->LightEnable(4, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x10);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_bTextRender)
	{
		_tchar	tCoinSlotEvent[MAX_PATH] = L"Press 'G' Button (-1 Coin)";
		
		Render_Font(L"LeeSoonSin", tCoinSlotEvent, &_vec2(_float(WINCX / 2 - 240), _float(WINCY / 2 + 200.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	m_pTextureCom->Set_Texture(0);//m_pAnimationCom->m_iMotion);	// 텍스처 정보 세팅을 우선적으로 한다.
	SetUp_Material();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->LightEnable(0, TRUE);
	m_pGraphicDev->LightEnable(1, TRUE);
	m_pGraphicDev->LightEnable(2, TRUE);
	m_pGraphicDev->LightEnable(3, TRUE);
	m_pGraphicDev->LightEnable(4, FALSE);
}

void CGacha_Machine::Collision_Event()
{
	CScene  *pScene = ::Get_Scene();
	NULL_CHECK_RETURN(pScene, );
	CLayer * pLayer = pScene->GetLayer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );
	CGameObject *pGameObject = nullptr;

	pGameObject = pLayer->Get_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, );
	CTransform *pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	if (m_pColliderCom->Check_Collision(this, pGameObject, 2, 2))
	{
		if (Engine::Key_Down(DIK_G))
		{
			m_bUseCoin = true;
			
			CCharacterInfo* pInfo = dynamic_cast<CCharacterInfo*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_CharacterInfoCom", ID_STATIC));

			if (m_bUseCoin)
			{
				pInfo->Use_Coin();
				m_bUseCoin = false;
			}

			Open_Event(pGameObject);
		}		
	}
}

HRESULT CGacha_Machine::Open_Event(CGameObject * pPlayer)
{
	CScene* pScene = ::Get_Scene();
	CLayer* pMyLayer = pScene->GetLayer(L"Layer_GameLogic");

	CDynamic_Transform *pTransform = dynamic_cast<CDynamic_Transform*>(pPlayer->Get_Component(L"Proto_DynamicTransformCom", ID_DYNAMIC));

	_vec3 vObjPos;
	_vec3 vPlayerPos;

	pTransform->Get_Info(INFO_POS, &vObjPos);
	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);

	if (m_pColliderCom->Check_Sphere_InterSect(vObjPos, vPlayerPos, 1.f, 1.f) == true)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		CUI_Frame* pUIFrame = nullptr;

		CLayer* pMonsterLayer = pScene->GetLayer(L"Layer_GameLogic");

		CLayer*	 pUILayer = pScene->GetLayer(L"Layer_UI");
		pUIFrame = dynamic_cast<CUI_Frame*>(pUILayer->Get_GameObject(L"Frame"));

		if (pUIFrame == nullptr)
			return E_FAIL;

		_tchar* ItemName = new _tchar[100];
		wstring szName = L"Item%d";
		wsprintfW(ItemName, szName.c_str(), pUIFrame->Get_ItemIndex());
		pMyLayer->AddNameList(ItemName);

		_uint iRand = rand() % 50;

		_tchar BatName[50];
		swprintf_s(BatName, L"SupriseBat%d", iRand);
		wstring szBatName = L"";
		szBatName += BatName;

		CGameObject* pGameObj = nullptr;

		if (rand() % 2 == 0)
		{
			pGameObj = READY_LAYER_POS(pGameObj, CHealthPotion, pMyLayer, m_pGraphicDev, ItemName, (_uint)vPos.x + 3, (_uint)vPos.z);
		}
		else if (rand() % 2 == 1)
		{
			pGameObj = READY_LAYER_POS(pGameObj, CCoin, pMyLayer, m_pGraphicDev, ItemName, (_uint)vPos.x + 3, (_uint)vPos.z);
		}
		else if (rand() % 2 == 2)
		{		// FatBat 1마리만 나오게 하면 꽝 역할로 딱인데 ㅠ
			CGameObject*	pGameObject = nullptr;
			pGameObject = CSpider::Create(m_pGraphicDev, (_uint)vPos.x + 3, (_uint)vPos.y);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pMonsterLayer->Add_GameObject(BatName, pGameObject), E_FAIL);
		}
	}

	return S_OK;
}

HRESULT CGacha_Machine::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_CoinSlotTexture", m_mapComponent, ID_STATIC);
	m_pAnimationCom = CAbstractFactory<CAnimation>::Clone_Proto_Component(L"Proto_AnimationCom", m_mapComponent, ID_STATIC);
	m_pColliderCom = CAbstractFactory<CCollider>::Clone_Proto_Component(L"Proto_ColliderCom", m_mapComponent, ID_STATIC);

	return S_OK;
}

HRESULT CGacha_Machine::SetUp_Material(void)
{
	_vec3 vPlayerPos, vPos;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransformCom);

	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fMtoPDistance = sqrtf((powf(vPlayerPos.x - vPos.x, 2)
		+ powf(vPlayerPos.y - vPos.y, 2)
		+ powf(vPlayerPos.z - vPos.z, 2)));


	D3DMATERIAL9		tMtrl;
	if (fMtoPDistance < 15.f)
	{
		_float fDistance = ((1.f - fMtoPDistance / 15.f)) + 0.5f;
		max(fDistance, 0.5f);
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));
		tMtrl.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Emissive = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Power = 0.f;
	}
	else
	{
		_float fDistance = 0.4f;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));
		tMtrl.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Emissive = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tMtrl.Power = 0.f;
	}


	m_pGraphicDev->SetMaterial(&tMtrl);
}

void CGacha_Machine::Set_Light_Obj()
{
	_vec3 vPlayerPos, vPos;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_DynamicTransformCom", ID_DYNAMIC));
	if (pPlayerTransformCom == nullptr)
		return;

	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fMtoPDistance = sqrtf((powf(vPlayerPos.x - vPos.x, 2)
		+ powf(vPlayerPos.y - vPos.y, 2)
		+ powf(vPlayerPos.z - vPos.z, 2)));

	if (fMtoPDistance < 15.f)
	{
		_float fDistance = ((1.f - fMtoPDistance / 15.f)) * 2;
		max(fDistance, 0.1f);

		D3DLIGHT9		tLightInfo4;
		ZeroMemory(&tLightInfo4, sizeof(D3DLIGHT9));
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		tLightInfo4.Type = D3DLIGHT_SPOT;
		tLightInfo4.Diffuse = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Specular = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Ambient = D3DXCOLOR(fDistance, fDistance, fDistance, fDistance);
		tLightInfo4.Position = vPos;
		FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo4, 4), );
	}
	else
	{
		return;
	}
}

CGacha_Machine * CGacha_Machine::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iX, _uint iY)
{
	CGacha_Machine*	pInstance = new CGacha_Machine(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iX, iY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CGacha_Machine::Free(void)
{
	Engine::CGameObject::Free();
}
