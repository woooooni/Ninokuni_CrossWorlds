#pragma once

#include "Vehicle_Flying.h"

BEGIN(Client)
class CVehicle_Flying_EnemyBoto final : public CVehicle_Flying
{
protected:
	CVehicle_Flying_EnemyBoto(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVehicle_Flying_EnemyBoto(const CVehicle_Flying_EnemyBoto& rhs);
	virtual ~CVehicle_Flying_EnemyBoto() = default;

public:
	void Using_TextureComponent(_bool bUse) { m_bUseTextureCom = bUse; }
	void Set_TextureIndex(_uint iIndex) {
		if (5 < iIndex)
			return;
		m_iTextureIndex = iIndex; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT Render_ShadowDepth();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT	Ready_Colliders();
	virtual HRESULT Ready_States();

private:
	void Update_RiderState();

private:
	void On_Damaged(const COLLISION_INFO& tInfo);

private:
	class CUI_Minigame_WorldHP* m_pHP = { nullptr };
	class CUI_Minigame_Aim* m_pAim = { nullptr };
	_bool m_bUseRigidbody = { true };

	_bool m_bUseTextureCom = { false };
	_uint m_iTextureIndex = { 0 };

	_float m_fDistance = { 0.f }; // 플레이어와 Boto의 사이거리 -> UI목적

private:
	_float4 m_vDissolveColor = _float4(0.427f, 0.894f, 1.f, 1.f);
	_float  m_fDissolveTotal = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight = 0.f;
	_float  m_fDissolveSpeed = 5.f;

	_float3 m_vBloomPower = _float3(0.f, 0.f, 0.f);

	_bool   m_bIsRimUse = false;
	_float  m_fRimTimeAcc = 0.f;

private:
	class CTexture* m_pDissolveTextureCom = nullptr;

public:
	static CVehicle_Flying_EnemyBoto* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

