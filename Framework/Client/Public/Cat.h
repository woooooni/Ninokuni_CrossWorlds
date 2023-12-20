#pragma once

#include "Animals.h"

BEGIN(Client)

class CCat final : public CAnimals
{
private:
	explicit CCat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CCat(const CCat& rhs);
	virtual ~CCat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>&WorldMatrices);
	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>&WorldMatrices);

public:
	virtual HRESULT Ready_Components() override;

public:
	static CCat* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);

public:
	virtual void Free() override;
};

END
