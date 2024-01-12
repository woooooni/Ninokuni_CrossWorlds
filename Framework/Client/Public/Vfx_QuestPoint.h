#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_QuestPoint final : public CVfx
{
protected:
	CVfx_QuestPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_QuestPoint(const CVfx_QuestPoint& rhs);
	virtual ~CVfx_QuestPoint() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal*    pDecalObject = nullptr;
	class CParticle* pLeafObject  = nullptr;
	class CParticle* pCircle01Object  = nullptr;
	class CParticle* pCircle02Object = nullptr;

public:
	static CVfx_QuestPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END