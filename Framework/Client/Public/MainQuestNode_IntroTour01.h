#pragma once

#include "QuestNode_Base.h"

BEGIN(Client)

class CMainQuestNode_IntroTour01 final : public CQuestNode_Base
{
private:
	CMainQuestNode_IntroTour01();
	virtual ~CMainQuestNode_IntroTour01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pKuu = nullptr;

public:
	static CMainQuestNode_IntroTour01* Create();
	virtual void Free() override;
};

END