#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_FinalBattle01 final : public CQuestNode_Base
{
private:
	CMainQuestNode_FinalBattle01();
	virtual ~CMainQuestNode_FinalBattle01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	class CGameObject* m_pWitchWood = nullptr;
	_bool m_bIsDisAppear = false;

public:
	static CMainQuestNode_FinalBattle01* Create();
	virtual void Free() override;
};

