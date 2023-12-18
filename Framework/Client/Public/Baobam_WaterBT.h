#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CBaobam_Water;
class CBaobam_WaterBT final : public CBehaviorTree
{
private:
	CBaobam_WaterBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaobam_WaterBT(const CBaobam_WaterBT& rhs);
	virtual ~CBaobam_WaterBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CGameObject* pObject);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

	virtual void     Init_NodeStart();

private:
	_bool	IsZeroHp();
	_bool   IsHitAnim();
	_bool	IsAtk();
	_bool	IsChase();
	_bool	IsReturn();

private:
	CBaobam_Water* m_pBaobam_Water = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

	_bool m_BIsChase = false;


public:
	static CBaobam_WaterBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
