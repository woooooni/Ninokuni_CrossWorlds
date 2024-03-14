#pragma once

#include "QuestNode_Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CSubQuestNode_Windmill10 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Windmill10();
	virtual ~CSubQuestNode_Windmill10() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pVerde = nullptr;
	CGameObject* m_pKuu = nullptr;
	CGameObject* m_pQuestObject = nullptr;

	_bool m_bIsFadeOut = false;

public:
	static CSubQuestNode_Windmill10* Create();
	virtual void Free() override;
};
END