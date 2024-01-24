#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_LeafSlam final : public CVfx
{
private: // 망치에 트레일 추가 필요
	enum TYPE {
		TYPE_ET1_D_CIRCLE,
		// --------------------
		TYPE_ET2_D_CRACK,
		TYPE_ET2_E_SPRINGUP,

		TYPE_ET2_P_STONES,
		TYPE_ET2_P_CIRCLES,
		TYPE_ET2_E_CIRCLELINE,

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_LeafSlam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_LeafSlam(const CVfx_Destroyer_Skill_LeafSlam& rhs);
	virtual ~CVfx_Destroyer_Skill_LeafSlam() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal*  m_pEt1_Decal    = nullptr;
	class CEffect* m_pEt2_SpringUp = nullptr;

public:
	static CVfx_Destroyer_Skill_LeafSlam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END