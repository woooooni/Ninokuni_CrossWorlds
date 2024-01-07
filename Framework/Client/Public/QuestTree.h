#pragma once

#include "Client_Defines.h"
#include "BTNode.h"
#include "Base.h"

BEGIN(Client)

class CQuestTree abstract : public CBase
{
protected:
	CQuestTree();
	virtual ~CQuestTree() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);

protected:
	CBTNode* m_pRootNode = nullptr;
	CBTNode* m_pCurNode = nullptr;

	_int m_iQuestIndex = 0;
	_int m_iCurQuestIndex = 0;

public:
	virtual void Free() override;
};

END

