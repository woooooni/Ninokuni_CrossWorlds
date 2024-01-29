#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_Biplane_3 final : public CSkill
{	
private:
	CSkill_Biplane_3();
	virtual ~CSkill_Biplane_3() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual _bool Use_Skill() override;

private:
	void Shoot_GuidedMissile();

public:
	static CSkill_Biplane_3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Free() override;
};

END

