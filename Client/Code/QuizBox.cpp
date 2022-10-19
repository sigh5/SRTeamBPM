#include "stdafx.h"
#include "..\Header\QuizBox.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Player_Dead_UI.h"

CQuizBox::CQuizBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI_Base(pGraphicDev)
{
}

CQuizBox::CQuizBox(const CUI_Base & rhs)
	: CUI_Base(rhs)
{
}

CQuizBox::~CQuizBox()
{
}

HRESULT CQuizBox::Ready_Object(_bool _bRenderQuiz)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	Set_OrthoMatrix(1024.f, 512.f, 0.f, 0.f);

	_vec3	vScale = { m_fSizeX * 0.7f, m_fSizeY * 0.7f, 1.f };

	m_pTransCom->Set_Scale(&vScale);
	m_pTransCom->Set_Pos(m_fX, m_fY, 0.05f);

	m_bRenderQuiz = _bRenderQuiz;

	return S_OK;
}

_int CQuizBox::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	
	Quiz_Answer();
	
	if (!m_bRandom)
	{
		m_iRandom = rand() % 3;
		m_bRandom = true;
	}	

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CQuizBox::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CQuizBox::Render_Obejct(void)
{
	if (m_bRenderQuiz)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

		_matrix		OldViewMatrix, OldProjMatrix;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		_matrix		ViewMatrix;

		ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);

		_matrix		matProj;

		Get_ProjMatrix(&matProj);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		if (m_bFalseAnswer)
		{			
			++m_iCount;
			
			_tchar*	pString = L"";

			switch (m_iRandom)
			{
			case 0:
			{
				pString = L"How about using your brain?";
				break;
			}

			case 1:
			{
				pString = L"복습을 생활화 합시다.";
				break;
			}

			case 2:
			{
				pString = L"이걸...틀려?";
				break;
			}

			case 3:
			{
				pString = L"뇌자극책을 다시 봅시다.";
				break;
			}
			}

			if(!m_bTest)
			Render_Font(L"LeeSoonSin", pString, &_vec2(700.f, 700.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
			
			if (m_iCount > 50)
			{
				m_bTest = true;
				m_bFalseAnswer = false;
			}
		}

		m_pTextureCom->Set_Texture(0);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	//CGameObject::Render_Obejct();
}

void CQuizBox::Quiz_Answer(void)
{
	CPlayer_Dead_UI* pPlayerDead = static_cast<CPlayer_Dead_UI*>(Engine::Get_GameObject(L"Layer_UI", L"Dead_UI"));

	if (Engine::Key_Down(DIK_1))
	{		
		m_bFalseAnswer = true;
		m_iCount = 0;
		m_bTest = false;
		m_bRandom = false;
	}
	Engine::Key_InputReset();

	if (Engine::Key_Down(DIK_2))
	{
		m_bFalseAnswer = true;
		m_iCount = 0;
		m_bTest = false;
		m_bRandom = false;
	}
	Engine::Key_InputReset();

	if (Engine::Key_Down(DIK_3))
	{
		m_iCount = 0;
		m_bTest = false;
		m_bRandom = false;
		m_bFalseAnswer = false;
		m_bRenderQuiz = false;
		pPlayerDead->Set_Render(false);
		pPlayerDead->Set_RenderIn(true);
		pPlayerDead->Set_BGM(false);
	}		
	Engine::Key_InputReset();
		
	if (Engine::Key_Down(DIK_4))
	{
		m_bFalseAnswer = true;
		m_iCount = 0;
		m_bTest = false;
		m_bRandom = false;
	}
	Engine::Key_InputReset();

	if (Engine::Key_Down(DIK_5))
	{
		m_bFalseAnswer = true;
		m_iCount = 0;
		m_bTest = false;
		m_bRandom = false;
	}
	Engine::Key_InputReset();
}

HRESULT CQuizBox::Add_Component(void)
{
	m_pBufferCom = CAbstractFactory<CRcTex>::Clone_Proto_Component(L"Proto_RcTexCom", m_mapComponent, ID_STATIC);
	m_pTransCom = CAbstractFactory<CTransform>::Clone_Proto_Component(L"Proto_TransformCom", m_mapComponent, ID_DYNAMIC);
	m_pTextureCom = CAbstractFactory<CTexture>::Clone_Proto_Component(L"Proto_QuizBox1_Texture", m_mapComponent, ID_STATIC);

	return S_OK;
}

CQuizBox * CQuizBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _bRenderQuiz)
{
	CQuizBox* pInstance = new CQuizBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_bRenderQuiz)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CQuizBox::Free(void)
{
	CGameObject::Free();
}
