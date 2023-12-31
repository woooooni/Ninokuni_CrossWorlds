#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CPumpkinCandle;
class CPumpkinCandleBT final : public CBehaviorTree
{
private:
	CPumpkinCandleBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPumpkinCandleBT(const CPumpkinCandleBT& rhs);
	virtual ~CPumpkinCandleBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CGameObject* pObject);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

	virtual void     Init_NodeStart();

private:
	_bool	IsZeroHp();
	_bool   IsWeak();
	_bool	IsAtk();
	_bool	IsChase();
	_bool	IsReturn();

private:
	CPumpkinCandle* m_pPumpkinCandle = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

	_bool m_BIsChase = false;


public:
	static CPumpkinCandleBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
