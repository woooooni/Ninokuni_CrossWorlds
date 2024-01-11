#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_KingCall final : public CQuestTree
{
private:
	CMainQuest_KingCall();
	virtual ~CMainQuest_KingCall() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_KingCall* Create();
	virtual void Free() override;
};

END

