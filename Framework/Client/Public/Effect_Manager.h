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
	HRESULT Generate_Effect(const wstring& strEffectName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner = nullptr, class CEffect** ppOut = nullptr, _bool bDelet = true);
	HRESULT Generate_Decal(const wstring& strDecalName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner = nullptr, class CDecal** ppOut = nullptr, _bool bDelet = true);
	HRESULT Generate_Vfx(const wstring& strVfxName, _matrix WorldMatrix, CGameObject* pOwner = nullptr, class CVfx** ppOut = nullptr);

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
	HRESULT Ready_Proto_Vfx_Monster();

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