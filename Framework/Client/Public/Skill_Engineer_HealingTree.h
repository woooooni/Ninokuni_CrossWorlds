#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_Engineer_HealingTree final : public CSkill
{	
private:
	CSkill_Engineer_HealingTree();
	virtual ~CSkill_Engineer_HealingTree() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual _bool Use_Skill() override;

public:
	static CSkill_Engineer_HealingTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Free() override;
};

END

