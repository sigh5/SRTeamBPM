#pragma once
#include "Scene.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;
END



class CToolTest : public CScene
{
private:
	explicit CToolTest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolTest() override;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;

	virtual void Free() override;
	static CToolTest* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	// Tool 마다 매개 변수로써 저장경로를 잡아주기 위해 멤버 변수로 하나씩 추가
private:
	wstring m_PpDirectory = L"../../Data/PlayerPos.dat";

private:
	class CDynamicCamera* m_pCam = nullptr;
};

