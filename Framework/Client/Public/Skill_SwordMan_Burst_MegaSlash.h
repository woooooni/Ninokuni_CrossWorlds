#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_SwordMan_Burst_MegaSlash final : public CSkill
{	
private:
	CSkill_SwordMan_Burst_MegaSlash();
	virtual ~CSkill_SwordMan_Burst_MegaSlash() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual _bool Use_Skill() override;

public:
	static CSkill_SwordMan_Burst_MegaSlash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Free() override;
};

END

