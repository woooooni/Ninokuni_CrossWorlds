#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CShadow_Thief;
class CShadow_ThiefBT final : public CBehaviorTree
{
private:
	CShadow_ThiefBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShadow_ThiefBT(const CShadow_ThiefBT& rhs);
	virtual ~CShadow_ThiefBT() = default;

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
	CShadow_Thief* m_pShadow_Thief = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

	_bool m_BIsChase = false;


public:
	static CShadow_ThiefBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
