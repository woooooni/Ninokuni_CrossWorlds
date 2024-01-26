#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_ExplosionShot final : public CVfx
{
private:
	enum TYPE {
		// 경고
		TYPE_ET1_D_RECT,
		// ------------------------------------------
		// 총알 발사 -> 총알 뒤에 트레일 및 반짝이 가는길에 몇개 뒤에 흘림
		// 번쩍하는 파티클
		TYPE_ET1_E_CIRCLELINE,// 점점 커지는 원라인 이펙트 -> 색 회색으로 변함
		TYPE_ET1_E_CIRCLES,   // 작은 원 파티클들 몇개
		TYPE_ET1_P_FIRE,      // 불꽃 파티클
		TYPE_ET1_P_SMOKE,     // 연기 파티클
		// ------------------------------------------
		TYPE_ET2_D_CIRCLE,    // 푸른 원 데칼
		TYPE_ET2_P_DOME,      // 붉은 원 이펙트
		TYPE_ET2_P_FIRE,      // 폭발 불꽃 파티클
		TYPE_ET2_E_CIRCLELINE,// 회오리 이펙트
		TYPE_ET2_P_CIRCLES,   // 작은 반짝이 파티클
		TYPE_ET2_P_SMOKE,     // 연기 파티클

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_ExplosionShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_ExplosionShot(const CVfx_Engineer_Skill_ExplosionShot& rhs);
	virtual ~CVfx_Engineer_Skill_ExplosionShot() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_ExplosionShot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END