#pragma once

#include "Vehicle_Flying.h"

BEGIN(Client)
class CVehicle_Flying_Biplane final : public CVehicle_Flying
{
public:
	enum BIPLANE_TRAIL { LEFT_WING, RIGHT_WING, TAIL, BIPLANE_TRAIL_END };

private:
	CVehicle_Flying_Biplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVehicle_Flying_Biplane(const CVehicle_Flying_Biplane& rhs);
	virtual ~CVehicle_Flying_Biplane() = default;

public:
	void Set_Players(_bool bIsPlayers) { m_bIsPlayers = bIsPlayers; }

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

public:
	void Start_Trail(BIPLANE_TRAIL eTrailType = BIPLANE_TRAIL::BIPLANE_TRAIL_END);
	void Generate_Trail(const wstring& strDiffuseTextureName, const wstring& strAlphaTextureName, const wstring& strDistortionTextureName, const _float4& vColor, _uint iVertexCount, BIPLANE_TRAIL eTrailType = BIPLANE_TRAIL::BIPLANE_TRAIL_END);
	void Stop_Trail(BIPLANE_TRAIL eTrailType = BIPLANE_TRAIL::BIPLANE_TRAIL_END);

	class CTrail* Get_Trail(BIPLANE_TRAIL eTrailType) 
	{ 
		if (eTrailType >= BIPLANE_TRAIL::BIPLANE_TRAIL_END)
			return nullptr;

		return m_pTrails[eTrailType]; 
	}

public:
	void Look_For_Target();

public:
	class CGameObject* Get_Target() { return m_pTarget; }

private:
	void Decide_Target();
	void Tick_Target();

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT	Ready_Colliders();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Trails();
	
	

private:
	void Update_RiderState();

private:
	void On_Damaged(const COLLISION_INFO& tInfo);

private:
	//class CTexture* m_pTextureCom = { nullptr };
	_bool m_bUseRigidbody = { true };
	_bool m_bIsPlayers = { false };

private:
	class CGameObject* m_pTarget = nullptr;
	class CCamera_Follow* m_pCameraFollow = nullptr;

private:
	class CTrail* m_pTrails[BIPLANE_TRAIL::BIPLANE_TRAIL_END] = {};
	class CUI_Grandprix_RaderIcon* m_pCameraIcon = { nullptr };
	class CUI_Grandprix_RaderIcon* m_pRaderIcon = { nullptr };

public:
	static CVehicle_Flying_Biplane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

