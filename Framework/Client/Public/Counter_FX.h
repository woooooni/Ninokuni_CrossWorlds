#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCounter_FX final : public CGameObject
{
private:
	explicit CCounter_FX(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCounter_FX(const CCounter_FX& rhs);
	virtual ~CCounter_FX() = default;

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
	

private:
	_float3 m_vBloomPower = { 0.f, 0.f, 0.f };
	_float4 m_vRimColor = {};
	_float m_fAlpha = 1.f;
	_float m_fAlphaSpeed = 1.f;
	_float m_fBlurPower = 0.f;
	Matrix m_WorldMatrix = Matrix::Identity;

private:
	_float m_fAccDeletionTime = 0.f;
	_float m_fDeletionTime = 2.f;

	_float m_fScaleSpeed = 3.f;

	Vec3 m_vInitScale = { 1.f, 1.f, 1.f };

public:
	virtual HRESULT Ready_Components() override;

public:
	static CCounter_FX* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

