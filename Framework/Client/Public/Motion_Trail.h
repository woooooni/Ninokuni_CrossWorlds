#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CMotion_Trail final : public CGameObject
{
private:
	explicit CMotion_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMotion_Trail(const CMotion_Trail& rhs);
	virtual ~CMotion_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	

private:
	TWEEN_DESC m_TweenDesc = {};
	class CModel* m_pModelCom = nullptr;
	class CModel* m_pRenderModelCom = nullptr;
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	


	_float3 m_vBloomPower = { 0.f, 0.f, 0.f };
	_float4 m_vRimColor = {};
	_float m_fAlpha = 1.f;
	_float m_fAlphaSpeed = 1.f;
	_float m_fBlurPower = 0.f;
	Matrix m_WorldMatrix = Matrix::Identity;

public:
	virtual HRESULT Ready_Components() override;

public:
	static CMotion_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

