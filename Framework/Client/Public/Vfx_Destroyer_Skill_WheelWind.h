#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_WheelWind final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE, // 0
		// --------------------
		TYPE_ET2_E_TORNADO, // 13 돌기 시작
		// --------------------

		TYPE_ET3_P_FIRE,
		TYPE_ET3_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_WheelWind(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_WheelWind(const CVfx_Destroyer_Skill_WheelWind& rhs);
	virtual ~CVfx_Destroyer_Skill_WheelWind() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pEt1_Decal = nullptr;

public:
	static CVfx_Destroyer_Skill_WheelWind* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END