#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx abstract : public CGameObject
{
protected:
	CVfx(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx(const CVfx& rhs);
	virtual ~CVfx() = default;

public:
	void Set_Owner(CGameObject* pGameObject) { m_pOwnerObject = pGameObject; }
	void Set_WorldMatrix(_matrix WorldMatrix) { XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix); }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_uint Get_MaxCount() { return m_iMaxCount; }
	_int* Get_FrameTriger() { return m_pFrameTriger; }
	_float3* Get_PositionOffset() { return m_pPositionOffset; }
	_float3* Get_ScaleOffset()    { return m_pScaleOffset; }
	_float3* Get_RotationOffset() { return m_pRotationOffset; }

protected:
	virtual HRESULT Ready_Components() override;

protected:
	_bool m_isCloned = { false };

	_float4x4 m_WorldMatrix;
	_bool m_bFinish = false;

protected:
	_uint  m_iCount    = 0;
	_uint  m_iMaxCount = 0;
	_float m_fTimeAcc  = 0.f;

protected:
	class CGameObject* m_pOwnerObject = nullptr;
	_int  m_bOwnerStateIndex = -1;
	_bool m_bOwnerTween = true;
	_uint m_iOwnerFrame = 0;

protected:
	_int* m_pFrameTriger = nullptr;
	_float3* m_pPositionOffset = nullptr;
	_float3* m_pScaleOffset    = nullptr;
	_float3* m_pRotationOffset = nullptr;

public:
	virtual void Free() override;
};

END