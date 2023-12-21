#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CIceBearMan;
class CIceBearManBT final : public CBehaviorTree
{
private:
	CIceBearManBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIceBearManBT(const CIceBearManBT& rhs);
	virtual ~CIceBearManBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CGameObject* pObject);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

	virtual void     Init_NodeStart();

private:
	_bool	IsZeroHp();
	_bool   IsHitAnim();
	_bool	IsAtkRound();
	_bool	IsChase();
	_bool	IsReturn();

private:
	CIceBearMan* m_pIceBearMan = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

	_bool m_BIsChase = false;


public:
	static CIceBearManBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
