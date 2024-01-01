#pragma once

#include "Client_Defines.h"
#include "MonsterBT.h"

BEGIN(Client)

class CIceBearMan;
class CIceBearManBT final : public CMonsterBT
{
private:
	CIceBearManBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIceBearManBT(const CIceBearManBT& rhs);
	virtual ~CIceBearManBT() = default;

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
	CIceBearMan* m_pIceBearMan = nullptr;

	_bool m_BIsChase = false;


public:
	static CIceBearManBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
