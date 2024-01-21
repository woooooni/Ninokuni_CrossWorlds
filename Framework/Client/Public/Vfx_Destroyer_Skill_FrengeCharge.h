#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_FrengeCharge final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_P_SPARCKE, // 0
		TYPE_ET1_E_TRAIL,
		TYPE_ET1_P_CIRCLES,
		// --------------------
		TYPE_ET2_D_CRACK, // 15
		TYPE_ET2_E_ELECT,
		// --------------------
		TYPE_ET3_E_CIRCLELINE, // 27
		TYPE_ET3_P_CIRCLES,
		TYPE_ET3_P_CIRCLESRIGID,
		// -------------------- // 61이 애니메이션 마지막 프레임
		TYPE_ET4_D_CIRCLE,
		TYPE_ET4_E_ELECT,
		TYPE_ET4_P_CIRCLESRIGID,
		TYPE_ET4_P_STONE,
		// X 6번 추가 반복 공격 횟수만큼

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_FrengeCharge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_FrengeCharge(const CVfx_Destroyer_Skill_FrengeCharge& rhs);
	virtual ~CVfx_Destroyer_Skill_FrengeCharge() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Destroyer_Skill_FrengeCharge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END