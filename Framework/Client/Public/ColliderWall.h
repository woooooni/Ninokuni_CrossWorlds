#pragma once

#include "StaticObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CColliderWall final : public CStaticObject
{
private:
	explicit CColliderWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CColliderWall(const CStaticObject& rhs);
	virtual ~CColliderWall() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

	virtual HRESULT Ready_Components();

private:
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

public:
	static CColliderWall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag,
		_int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
