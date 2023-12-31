#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CHumanChild01;
class CHumanChild01BT final : public CBehaviorTree
{
	CHumanChild01BT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHumanChild01BT(const CHumanChild01BT& rhs);
	virtual ~CHumanChild01BT() = default;

public:
	virtual HRESULT Initialize_Prototype(CGameObject* pObject);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

	virtual void     Init_NodeStart();

private:
	_bool	IsOneWay();
	_bool	IsTwoWay();

private:
	CGameNpc* m_pNpc = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

public:
	static CHumanChild01BT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
