#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CDoor_Enter_FX final : public CGameObject
{
private:
	CDoor_Enter_FX(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDoor_Enter_FX(const CDoor_Enter_FX& rhs);
	virtual ~CDoor_Enter_FX() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_Collider();

	

private:
	class CTransform* m_pTransformCom = nullptr;
	class CModel* m_pModelCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTexture* m_pDissolveTextureCom = nullptr;
	class CShader* m_pShaderCom = nullptr;

private:
	_bool m_bDissolve = true;

private:
	_bool m_bPlayAnimation = false;
	_float m_fWaitTime = 2.f;
	_float m_fAccWait = 0.f;


private:
	_float4 m_vDissolveColor = _float4(0.427f, 0.894f, 1.f, 1.f);
	_float  m_fDissolveTotal = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight = 0.f;
	_float  m_fDissolveSpeed = 5.f;



public:
	static CDoor_Enter_FX* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

