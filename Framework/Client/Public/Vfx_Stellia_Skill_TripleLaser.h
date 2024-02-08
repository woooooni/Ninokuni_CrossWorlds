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

		// 중간 상단 구슬 // 전기 한두번 나옴 13 - a
		TYPE_ET2_E_MARBLE_MU,
		// 좌측 하단 구슬 // 38 - a
		TYPE_ET2_E_MARBLE_LD,
		// 우측 하단 구슬 // 59 - a
		TYPE_ET2_E_MARBLE_RD,

		// 동시에 각각의 구슬에서 빔 발사 -> 앞쪽 한 곳으로 발사 / 전기 // 90

		// 반짝이

		TYPE_END
	};

	// 구슬이 크게 먼저 생기고 주변에 아우라가 생긴다. 구슬은 점차 작아진다. 구슬에 전기 치지직 흘러요
	//  구슬이 다 똑같은 크기가 되면 앞쪽 아래로 3개의 레이저 발사가 된다. 십자가 메시인듯.
	// 레이지거 압쪽방향으로 나아가면서 얇아짐 구ㅜ슬은 중반쯤ㅇ 사라짐. 레이저에 전기랑 반짝이 있네.

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