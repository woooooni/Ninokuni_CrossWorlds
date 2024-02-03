#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_Biplane_Burst final : public CSkill
{	
private:
	CSkill_Biplane_Burst();
	virtual ~CSkill_Biplane_Burst() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual _bool Use_Skill() override;

private:
	void Shoot_BlackHole();

public:
	static CSkill_Biplane_Burst* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Free() override;
};

END

