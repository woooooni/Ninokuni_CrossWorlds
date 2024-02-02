#pragma once

#include "QuestNode_Base.h"
#include "StateMachine.h"

class CMainQuestNode_WitchForest06 final : public CQuestNode_Base
{
private:
	CMainQuestNode_WitchForest06();
	virtual ~CMainQuestNode_WitchForest06() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	class CGameObject* m_pWitchWood = nullptr;

public:
	static CMainQuestNode_WitchForest06* Create();
	virtual void Free() override;
};

