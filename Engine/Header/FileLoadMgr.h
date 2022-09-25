#pragma once
#include "Base.h"
#include "Engine_Include.h"
BEGIN(Engine)


class ENGINE_DLL CFileLoadMgr :public CBase
{
	DECLARE_SINGLETON(CFileLoadMgr)

private:
	explicit CFileLoadMgr();
	virtual ~CFileLoadMgr();







public:
	virtual void		Free(void);
};

END