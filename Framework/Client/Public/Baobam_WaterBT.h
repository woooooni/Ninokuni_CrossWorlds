#pragma once

#include "Client_Defines.h"
#include "MonsterBT.h"

BEGIN(Client)

class CBaobam_Water;
class CBaobam_WaterBT final : public CMonsterBT
{
private:
	CBaobam_WaterBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaobam_WaterBT(const CBaobam_WaterBT& rhs);
	virtual ~CBaobam_WaterBT() = default;

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
	CBaobam_Water* m_pBaobam_Water = nullptr;

public:
	static CBaobam_WaterBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
