#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Client)

class CVfx_Stellia_Skill_Rage01Shield_All final : public CVfx
{
private:
	enum TYPE {
		TYPE_V_E_SHIELD_ALL,
		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage01Shield_All(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage01Shield_All(const CVfx_Stellia_Skill_Rage01Shield_All& rhs);
	virtual ~CVfx_Stellia_Skill_Rage01Shield_All() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_bool m_bIsStart = false;

public:
	static CVfx_Stellia_Skill_Rage01Shield_All* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END