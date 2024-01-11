#pragma once

#include "Client_Defines.h"
#include "QuestTree.h"
#include "Base.h"

BEGIN(Client)

class CSubQuest final : public CBase
{
public:
	enum SUBQUEST_NAME { SUBQUEST_CHLOE_FINDCAT, SUBQUEST_VERDE_WINDMILL, SUBQUEST_END };

private:
	CSubQuest();
	virtual ~CSubQuest() = default;

public:
	HRESULT Initialize();
	void   Tick(const _float& fTimeDelta);
	void   LateTick(const _float& fTimeDelta);

public:
	void Set_SubQuestRunning(SUBQUEST_NAME eSQName, _bool bIsRunning);

private:
	vector<CQuestTree*> m_vecSQ;

public:
	static CSubQuest* Create();
	virtual void Free() override;
};


END

