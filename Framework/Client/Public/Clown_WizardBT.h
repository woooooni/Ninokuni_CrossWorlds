#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CClown_Wizard;
class CClown_WizardBT final : public CBehaviorTree
{
private:
	CClown_WizardBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CClown_WizardBT(const CClown_WizardBT& rhs);
	virtual ~CClown_WizardBT() = default;

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
	CClown_Wizard* m_pClown_Wizard = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

	_bool m_BIsChase = false;


public:
	static CClown_WizardBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
