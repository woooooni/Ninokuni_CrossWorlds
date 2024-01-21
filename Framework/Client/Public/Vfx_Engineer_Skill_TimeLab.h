#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_TimeLab final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE, // 0

		TYPE_ET1_E_TIME_BIG, // 16
		TYPE_ET1_P_LIGHT,
		TYPE_ET1_P_CIRCLES,

		TYPE_ET2_E_TIME_SMALL, // 28
		TYPE_ET2_P_LIGHTLINE,
		TYPE_ET2_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_TimeLab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_TimeLab(const CVfx_Engineer_Skill_TimeLab& rhs);
	virtual ~CVfx_Engineer_Skill_TimeLab() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_TimeLab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END