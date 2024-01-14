#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_JumpDown final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_START_WARNING,

		TYPE_D_PREACT_HANDCRACK_LEFT,
		TYPE_D_PREACT_HANDCRACK_RIGHT,
		TYPE_P_PREACT_SMOKE,
		TYPE_P_PREACT_CIRCLES,

		TYPE_D_ATTACK_DOWNCRACK,
		TYPE_E_ATTACK_SPRINGUP,
		TYPE_P_ATTACK_SMOKE,
		TYPE_E_ATTACK_CIRCLELINE,

		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_JumpDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_JumpDown(const CVfx_Glanix_Skill_JumpDown& rhs);
	virtual ~CVfx_Glanix_Skill_JumpDown() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Glanix_Skill_JumpDown* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END