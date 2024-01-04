#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_Destroyer_BrutalStrike final : public CSkill
{	
private:
	CSkill_Destroyer_BrutalStrike();
	virtual ~CSkill_Destroyer_BrutalStrike() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual void Use_Skill() override;

public:
	static CSkill_Destroyer_BrutalStrike* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Free() override;
};

END

