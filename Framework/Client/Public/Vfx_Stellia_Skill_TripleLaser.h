#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_TripleLaser final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_WARNING_CIRCLE,
		// -------------------------
		TYPE_ET2_E_MARBLE_MU,
		TYPE_ET2_E_MARBLE_LINE_MU,

		TYPE_ET2_E_MARBLE_LD,
		TYPE_ET2_E_MARBLE_LINE_LD,

		TYPE_ET2_E_MARBLE_RD,
		TYPE_ET2_E_MARBLEE_LINE_RD,
		// -------------------------
		TYPE_ET3_E_ELECT,
		// -------------------------
		TYPE_EVENT_DELETE,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_TripleLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_TripleLaser(const CVfx_Stellia_Skill_TripleLaser& rhs);
	virtual ~CVfx_Stellia_Skill_TripleLaser() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	Vec4 m_vTargetPos = {};

	class CDecal* m_pWarningDecal = nullptr;

	class CEffect* m_pMarble_MU = nullptr;
	class CEffect* m_pMarble_LD = nullptr;
	class CEffect* m_pMarble_RD = nullptr;

	class CEffect* m_pMarble_Line_MU = nullptr;
	class CEffect* m_pMarble_Line_LD = nullptr;
	class CEffect* m_pMarble_Line_RD = nullptr;

	class CParticle* m_pElect_MU = nullptr;
	class CParticle* m_pElect_LD = nullptr;
	class CParticle* m_pElect_RD = nullptr;

	class CEffect* m_pElectLine_MU = nullptr;
	class CEffect* m_pElectLine_LD = nullptr;
	class CEffect* m_pElectLine_RD = nullptr;

	class CParticle* m_pCircles_MU = nullptr;
	class CParticle* m_pCircles_LD = nullptr;
	class CParticle* m_pCircles_RD = nullptr;

public:
	static CVfx_Stellia_Skill_TripleLaser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END