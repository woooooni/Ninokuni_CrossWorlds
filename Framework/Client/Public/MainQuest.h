#pragma once

#include "Client_Defines.h"
#include "QuestTree.h"
#include "Base.h"

BEGIN(Client)

class CMainQuest final : public CBase
{
private:
	CMainQuest();
	virtual ~CMainQuest() = default;

public:
	HRESULT Initialize();
	void   Tick(const _float& fTimeDelta);
	void   LateTick(const _float& fTimeDelta);

private:
	vector<CQuestTree*> m_vecMQ;

	_int m_iLevel = 0;

public:
	static CMainQuest* Create();
	virtual void Free() override;
};


END

