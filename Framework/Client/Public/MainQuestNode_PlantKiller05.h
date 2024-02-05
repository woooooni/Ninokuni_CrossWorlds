#pragma once

#include "QuestNode_Base.h"

BEGIN(Client)

class CMainQuestNode_PlantKiller05 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller05();
	virtual ~CMainQuestNode_PlantKiller05() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	class CRuby* m_pRuby = nullptr;

public:
	static CMainQuestNode_PlantKiller05* Create();
	virtual void Free() override;
};

END