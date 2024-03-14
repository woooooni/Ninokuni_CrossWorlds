#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Intro_Roar final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_P_SPARKLE,
		TYPE_ET1_P_SMOKE,

		TYPE_ET2_P_SPARKLE,
		TYPE_ET2_P_SMOKE,

		TYPE_ET3_P_SPARKLE,
		TYPE_ET3_P_SMOKE,

		TYPE_ET4_P_SPARKLE,
		TYPE_ET4_P_SMOKE,

		TYPE_ET5_P_SPARKLE,
		TYPE_ET5_P_SMOKE,
		
		TYPE_ET6_E_ROAR,
		
		TYPE_END
	};

protected:
	CVfx_Glanix_Intro_Roar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Intro_Roar(const CVfx_Glanix_Intro_Roar& rhs);
	virtual ~CVfx_Glanix_Intro_Roar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_float fTimeAcc = 0.f;
	_bool m_bRadialBlur = false;

public:
	static CVfx_Glanix_Intro_Roar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END