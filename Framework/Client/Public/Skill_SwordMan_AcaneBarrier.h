#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_SwordMan_AcaneBarrier final : public CSkill
{	
private:
	CSkill_SwordMan_AcaneBarrier();
	virtual ~CSkill_SwordMan_AcaneBarrier() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual _bool Use_Skill() override;

public:
	static CSkill_SwordMan_AcaneBarrier* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Free() override;
};

END

