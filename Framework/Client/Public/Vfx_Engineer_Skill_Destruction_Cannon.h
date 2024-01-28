#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_Destruction_Cannon final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_P_LIGHT,
		// ---------------------------
		TYPE_ET2_O_BULLET,
		TYPE_ET2_E_CIRCLELINES,

		TYPE_ET2_P_FIRE,
		TYPE_ET2_P_TWINKLE,
		TYPE_ET2_P_SMOKE,
		// ---------------------------
		TYPE_ET3_E_METAL,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_Destruction_Cannon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_Destruction_Cannon(const CVfx_Engineer_Skill_Destruction_Cannon& rhs);
	virtual ~CVfx_Engineer_Skill_Destruction_Cannon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	void Fire_Cannon();

public:
	static CVfx_Engineer_Skill_Destruction_Cannon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END