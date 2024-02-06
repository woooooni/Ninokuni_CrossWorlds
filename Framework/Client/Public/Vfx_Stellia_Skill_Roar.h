#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Roar final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_E_ROAR,
		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Roar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Roar(const CVfx_Stellia_Skill_Roar& rhs);
	virtual ~CVfx_Stellia_Skill_Roar() = default;

public:
	void Set_RoarFrame(_int iStartFrame, _int iFinishFrame)
	{ 
		if (nullptr == m_pFrameTriger)
			return;

		m_pFrameTriger[TYPE_ET1_E_ROAR] = iStartFrame;
		m_iFinishFrame = iFinishFrame;
	}

	void Set_RoarPosition(_float3 fPosition)
	{
		if (nullptr == m_pFrameTriger)
			return;

		m_pPositionOffset[TYPE_ET1_E_ROAR] = fPosition;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_int m_iFinishFrame = 90;

public:
	static CVfx_Stellia_Skill_Roar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END