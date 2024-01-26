#pragma once

#include "Client_Defines.h"
#include "Character_Projectile.h"

BEGIN(Client)

class CEngineer_Burst_DestructionCannon final : public CCharacter_Projectile
{
private:
	explicit CEngineer_Burst_DestructionCannon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEngineer_Burst_DestructionCannon(const CEngineer_Burst_DestructionCannon& rhs);
	virtual ~CEngineer_Burst_DestructionCannon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;


	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;
	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;


private:
	void Create_Cannon();
	void Fire_Cannon();
	void Delete_Cannon();

private:
	_bool m_bGenEffect = false;
	_bool m_bShot = false;

private:	
	class CTexture* m_pDissolveTextureCom = nullptr;

private:
	ANIMODEL_INSTANCE_DESC m_AnimInstanceDesc = {};

	_float4 m_vDissolveColor = _float4(0.427f, 0.894f, 1.f, 1.f);
	_float  m_fDissolveTotal = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight = 0.f;
	_float  m_fDissolveSpeed = 5.f;

private:

public:
	static CEngineer_Burst_DestructionCannon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

