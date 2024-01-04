#pragma once

#include "Skill.h"

BEGIN(Client)

class CSkill_Destroyer_WheelWind final : public CSkill
{	
private:
	CSkill_Destroyer_WheelWind();
	virtual ~CSkill_Destroyer_WheelWind() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameObject* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	static CSkill_Destroyer_WheelWind* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameObject* pOwner);
	virtual void Free() override;
};

END

