#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_BattleCry final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE, // X
		// --------------------
		// --------------------
		TYPE_ET2_E_CIRCLELINE,//2
		// --------------------
		// --------------------
		TYPE_ET3_E_DOME, // X
		// --------------------
		TYPE_ET3_D_FIRECRACK, // X

		TYPE_ET3_E_SPRINGUP,   //3
		TYPE_ET3_E_CIRCLELINE, // X

		TYPE_ET3_P_STONE,   // X
		TYPE_ET3_P_CIRCLES, // X

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_BattleCry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_BattleCry(const CVfx_Destroyer_Skill_BattleCry& rhs);
	virtual ~CVfx_Destroyer_Skill_BattleCry() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_int m_iType = -1;
	_float3 m_fMainColor;

private:
	class CDecal*  m_pEt1_Decal  = nullptr;
	class CEffect* m_pEt2_Line   = nullptr;
	class CEffect* m_pEt3_Dome   = nullptr;
	class CEffect* m_pEt3_Spring = nullptr;

public:
	static CVfx_Destroyer_Skill_BattleCry* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END