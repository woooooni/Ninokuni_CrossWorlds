#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CEngineer_Bullet final : public CGameObject
{
private:
	explicit CEngineer_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEngineer_Bullet(const CEngineer_Bullet& rhs);
	virtual ~CEngineer_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	virtual HRESULT Ready_Components() override;


private:
	class CModel* m_pModelCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

private:
	_float m_fAccDeletionTime = 0.f;
	_float m_fDeletionTime = 1.f;
	_float m_fMoveSpeed = 20.f;


public:
	static CEngineer_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

