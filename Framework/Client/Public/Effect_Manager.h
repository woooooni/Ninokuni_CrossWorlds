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
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strEffectMeshPath, const wstring& strEffectPath);
	void	Tick(_float fTimeDelta);

public:
	HRESULT Generate_Effect(const wstring& strPrototypeEffectName, _matrix RotationMatrix, _matrix WorldMatrix, _float fEffectDeletionTime, 
		class CGameObject* pOwner = nullptr, 
		class CEffect** ppOut = nullptr);
	HRESULT Generate_Vfx(const wstring& strPrototypeVfxName, _vector vPosition);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	HRESULT Ready_Proto_Effects(const wstring& strEffectPath);
	HRESULT Ready_Proto_Decal();
	HRESULT Ready_Proto_Vfx();

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