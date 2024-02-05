#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_ClawDouble final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_TRAIL_01,
		TYPE_P_CIRCLES_01,

		TYPE_E_TRAIL_02,
		TYPE_P_CIRCLES_02,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_ClawDouble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_ClawDouble(const CVfx_Stellia_Skill_ClawDouble& rhs);
	virtual ~CVfx_Stellia_Skill_ClawDouble() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pHand01Effect = nullptr;
	class CEffect* m_pHand02Effect = nullptr;

public:
	static CVfx_Stellia_Skill_ClawDouble* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END