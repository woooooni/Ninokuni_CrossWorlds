#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CStellia;
class CStelliaBT final : public CBehaviorTree
{
private:
	CStelliaBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStelliaBT(const CStelliaBT& rhs);
	virtual ~CStelliaBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CGameObject* pObject);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	_bool	IsZeroHp();
	_bool	IsChase();
	_bool	IsBerserk();

private:
	CStellia* m_pStellia = nullptr;

	_bool m_BIsChase = true;

public:
	static CStelliaBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
