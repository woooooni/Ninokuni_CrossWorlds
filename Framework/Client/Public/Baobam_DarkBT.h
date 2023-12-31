#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CBaobam_Dark;
class CBaobam_DarkBT final : public CBehaviorTree
{
private:
	CBaobam_DarkBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaobam_DarkBT(const CBaobam_DarkBT& rhs);
	virtual ~CBaobam_DarkBT() = default;

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
	CBaobam_Dark* m_pBaobam_Dark = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

	_bool m_BIsChase = false;


public:
	static CBaobam_DarkBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
