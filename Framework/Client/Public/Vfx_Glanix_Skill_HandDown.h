#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_HandDown final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_START_WARNING,

		TYPE_P_PREACT_BIGSMOKE,
		TYPE_E_PREACT_CIRCLELINE,
		TYPE_P_PREACT_CIRCLES,
		TYPE_E_PREACT_HANDCIRCLELEFT,
		TYPE_E_PREACT_HANDCIRCLERIGHT,

		TYPE_D_ATTACK_CRACK,
		TYPE_E_ATTACK_SPRINGUP,
		TTPE_P_ATTACK_SMOKE,
		TYPE_P_ATTACK_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_HandDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_HandDown(const CVfx_Glanix_Skill_HandDown& rhs);
	virtual ~CVfx_Glanix_Skill_HandDown() = default;

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

private:
	class CEffect* m_pHand01Effect = nullptr;
	class CEffect* m_pHand02Effect = nullptr;

	class CDecal* m_pWarningDecal = nullptr;
	class CEffect* m_SpringUpEffect = nullptr;

public:
	static CVfx_Glanix_Skill_HandDown* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END