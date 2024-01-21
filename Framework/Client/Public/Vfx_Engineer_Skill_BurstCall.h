#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_BurstCall final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE, // 23
		TYPE_ET1_E_LINE,

		TYPE_ET1_E_CIRCLELINE,
		TYPE_ET1_P_CIRCLES,
		// -------------------
		TYPE_ET2_D_FIRETRACE, //30
		TYPE_ET2_P_LIGHT,
		TYPE_ET2_P_SMOKE,

		TYPE_ET2_P_FIRE,
		TYPE_ET2_E_CIRCLELINE,
		TYPE_ET2_P_STONE,
		TYPE_ET2_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_BurstCall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_BurstCall(const CVfx_Engineer_Skill_BurstCall& rhs);
	virtual ~CVfx_Engineer_Skill_BurstCall() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_BurstCall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END