#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_Spawn final : public CVfx
{
private://* 착지하고 포효
	enum TYPE { // L"SKM_Glanix.ao|Glanix_Spawn_New"
		// 14 착지
		TYPE_ET1_P_CIRCLES,
		TYPE_ET1_P_SMOKE,
		TYPE_ET1_E_CIRCLELINE,

		// 44 포효시작 // 69 포효끝
		TYPE_ET2_E_ROAR,

		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_Spawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_Spawn(const CVfx_Glanix_Skill_Spawn& rhs);
	virtual ~CVfx_Glanix_Skill_Spawn() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_float m_fTimeAcc = 0.f;
	_bool m_bRadialBlur = false;

public:
	static CVfx_Glanix_Skill_Spawn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END