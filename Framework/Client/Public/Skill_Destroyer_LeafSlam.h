#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_Destroyer_LeafSlam final : public CSkill
{	
private:
	CSkill_Destroyer_LeafSlam();
	virtual ~CSkill_Destroyer_LeafSlam() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual void Use_Skill() override;

public:
	static CSkill_Destroyer_LeafSlam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Free() override;
};

END

