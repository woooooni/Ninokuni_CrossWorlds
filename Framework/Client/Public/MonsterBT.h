#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CMonster;
class CMonsterBT abstract : public CBehaviorTree
{
public:
	typedef struct tagBtMonsterDesc
	{
		CMonster* pOwner = nullptr;
		CTransform* pOwnerTransform = nullptr;
		CModel* pOwnerModel = nullptr;
	}BT_MONSTERDESC;

protected:
	CMonsterBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterBT(const CMonsterBT& rhs);
	virtual ~CMonsterBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonster* pOwner);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

protected:
	BT_MONSTERDESC m_tBTMonsterDesc = {};

public:
	virtual void Free() override;
};

END
