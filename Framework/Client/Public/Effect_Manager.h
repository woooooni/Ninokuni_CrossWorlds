#pragma once

#include "Client_Defines.h"
#include "Pool.h"
#include "Base.h"


BEGIN(Client)
class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strEffectMeshPath, const wstring& strEffectPath, const wstring& strDecalPath);
	void	Tick(_float fTimeDelta);

public:
	HRESULT Generate_Effect(const wstring& strEffectName, _matrix WorldMatrix, _matrix* pRotationMatrix = nullptr, CGameObject* pOwner = nullptr, class CEffect** ppOut = nullptr);
	HRESULT Generate_Effect(const wstring& strEffectName, _matrix WorldMatrix, _vector vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner = nullptr, class CEffect** ppOut = nullptr);
	HRESULT Generate_Decal(const wstring& strDecalName, _matrix WorldMatrix, _matrix* pRotationMatrix = nullptr, CGameObject* pOwner = nullptr, class CDecal** ppOut = nullptr);
	HRESULT Generate_Vfx(const wstring& strVfxName, _matrix WorldMatrix, _matrix* pRotationMatrix = nullptr, CGameObject* pOwner = nullptr, class CVfx** ppOut = nullptr);

	_matrix Calculate_WorldMatrixEffect(_matrix	ParentsWorldMatrix, _matrix	ChildWorldMatrix);
	_matrix Get_WorldMatrixEffect(_matrix OwnerWorldMatrix, _float3 fPositionOffset, _float3 fScaleOffset, _float3 fRotationOffset);
	_matrix Get_RotationMatrix(_float3 fRotationOffset);

private:						  
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	HRESULT Ready_Proto_Effects(const wstring& strEffectPath);
	HRESULT Ready_Proto_Decal(const wstring& strDecalPath);
	HRESULT Ready_Proto_Vfx();

private:
	HRESULT Ready_Proto_Vfx_UI();
	HRESULT Ready_Proto_Vfx_Player();

public:
	virtual void Free() override;
};

END

/* Proto_Effect

   Effect_

*/

/* Proto_Vfx

   Vfx_MouseClick

*/