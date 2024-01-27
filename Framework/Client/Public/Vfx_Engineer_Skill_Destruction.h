#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_Destruction final : public CVfx
{
private:
	enum TYPE {
		// 범위 표시
		TYPE_ET1_D_RECT,
		// ---------------------------
		// 대포 생성
		TYPE_ET2_O_CANNON,
		TYPE_ET2_E_METAL,
		TYPE_ET2_P_SMOKE,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_Destruction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_Destruction(const CVfx_Engineer_Skill_Destruction& rhs);
	virtual ~CVfx_Engineer_Skill_Destruction() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	void Create_Cannon();

public:
	static CVfx_Engineer_Skill_Destruction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END