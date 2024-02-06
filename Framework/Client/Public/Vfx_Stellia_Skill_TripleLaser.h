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

		// 중간 상단 구슬 // 전기 한두번 나옴
		TYPE_ET2_E_MARBLE_MU,
		// 좌측 하단 구슬
		TYPE_ET2_E_MARBLE_LD,
		// 우측 하단 구슬
		TYPE_ET2_E_MARBLE_RD,

		// 동시에 각각의 구슬에서 빔 발사 -> 앞쪽 한 곳으로 발사 / 전기

		// 반짝이

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
	class CDecal* m_pWarningDecal = nullptr;

public:
	static CVfx_Stellia_Skill_TripleLaser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END