#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_SwordMan_SpinningAssault final : public CSkill
{	
private:
	CSkill_SwordMan_SpinningAssault();
	virtual ~CSkill_SwordMan_SpinningAssault() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameObject* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	static CSkill_SwordMan_SpinningAssault* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameObject* pOwner);
	virtual void Free() override;
};

END

