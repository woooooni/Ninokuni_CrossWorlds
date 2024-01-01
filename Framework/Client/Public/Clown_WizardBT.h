#pragma once

#include "Client_Defines.h"
#include "MonsterBT.h"

BEGIN(Client)

class CClown_Wizard;
class CClown_WizardBT final : public CMonsterBT
{
private:
	CClown_WizardBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CClown_WizardBT(const CClown_WizardBT& rhs);
	virtual ~CClown_WizardBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonster* pOwner);
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
	class CClown_Wizard* m_pClown_Wizard = nullptr;

	_bool m_BIsChase = false;


public:
	static CClown_WizardBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
