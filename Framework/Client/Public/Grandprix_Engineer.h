#pragma once

#include "Grandprix_Enemy.h"

BEGIN(Client)
class CGrandprix_Engineer final : public CGrandprix_Enemy
{
private:
	CGrandprix_Engineer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrandprix_Engineer(const CGrandprix_Engineer& rhs);
	virtual ~CGrandprix_Engineer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Infinite(_bool bInfinite, _float fInfiniteTime) 
	{ 
		m_bInfinite = bInfinite;
		m_fInfiniteTime = fInfiniteTime;
		m_fAccInfinite = 0.f;
	}
	
protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	_bool m_bInfinite = true;
	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 99999999.f;


private:
	_float4 m_vDissolveColor = _float4(0.427f, 0.894f, 1.f, 1.f);
	_float  m_fDissolveTotal = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight = 0.f;
	_float  m_fDissolveSpeed = 5.f;

	class CTexture* m_pDissolveTexture = nullptr;

	_float3 m_vBloomPower = _float3(0.f, 0.f, 0.f);


private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

private:
	_float2 Transpose_ProjectionPosition();

private:
	class CModel* m_pCharacterPartModels[PART_TYPE::PART_END] = {};
	
public:
	static CGrandprix_Engineer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

