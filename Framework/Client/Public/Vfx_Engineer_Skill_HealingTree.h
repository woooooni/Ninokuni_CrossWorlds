#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_HealingTree final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE,
		TYPE_ET1_E_CIRCLELINE,
		// ---------------------
		TYPE_ET2_E_DOME,
		TYPE_ET2_E_CROSS,
		TYPE_ET2_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_HealingTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_HealingTree(const CVfx_Engineer_Skill_HealingTree& rhs);
	virtual ~CVfx_Engineer_Skill_HealingTree() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect*   m_pDome    = nullptr;
	class CEffect*   m_pCross   = nullptr;

public:
	static CVfx_Engineer_Skill_HealingTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END