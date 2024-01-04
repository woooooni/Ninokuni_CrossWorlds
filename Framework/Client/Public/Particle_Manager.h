#pragma once

#include "Client_Defines.h"
#include "Pool.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CParticle_Manager : public CBase
{
	DECLARE_SINGLETON(CParticle_Manager)

private:
	CParticle_Manager();
	virtual ~CParticle_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPatriclePath);
	void Tick(_float fTimeDelta);

public:
	HRESULT Generate_Particle(const wstring& strParticleName, _matrix WorldMatrix, _matrix* pRotationMatrix = nullptr, CGameObject* pOwner = nullptr, class CParticle** ppOut = nullptr);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	HRESULT Ready_Proto_Particles(const wstring& strParticlePath);

public:
	virtual void Free() override;
};

END

/* Proto_Particles

   Particle_Mouse_Circle
   Particle_Mouse_CircleLine
   Particle_Mouse_Leaf
   Particle_Mouse_Twinkle

*/