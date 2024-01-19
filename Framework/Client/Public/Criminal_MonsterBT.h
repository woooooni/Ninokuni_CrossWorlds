#pragma once

#include "Client_Defines.h"
#include "MonsterBT.h"

BEGIN(Client)

class CCriminal_Monster;
class CCriminal_MonsterBT final : public CMonsterBT
{
private:
	CCriminal_MonsterBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCriminal_MonsterBT(const CCriminal_MonsterBT& rhs);
	virtual ~CCriminal_MonsterBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonster* pOwner);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

	virtual void     Init_NodeStart();

private:
	_bool	IsZeroHp();
	_bool   IsHit();
	_bool	IsAtk();
	_bool	IsChase();
	_bool	IsReturn();

private:
	CCriminal_Monster* m_pCriminal_Monster = nullptr;

public:
	static CCriminal_MonsterBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
