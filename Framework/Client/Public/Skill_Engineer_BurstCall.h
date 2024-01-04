#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_Engineer_BurstCall final : public CSkill
{	
private:
	CSkill_Engineer_BurstCall();
	virtual ~CSkill_Engineer_BurstCall() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameObject* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	static CSkill_Engineer_BurstCall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameObject* pOwner);
	virtual void Free() override;
};

END

