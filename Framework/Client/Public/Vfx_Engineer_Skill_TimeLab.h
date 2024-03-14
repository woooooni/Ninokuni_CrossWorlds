#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_TimeLab final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE,
		// ---------------------------
		TYPE_ET1_E_TIME_BIG_FRAME,
		TYPE_ET1_E_TIME_BIG_MINUTE,
		TYPE_ET1_E_TIME_BIG_SECOND,
		// ---------------------------
		TYPE_ET1_P_LIGHT,   // 
		TYPE_ET1_P_CIRCLES, //
		// ---------------------------
		TYPE_ET2_E_TIME_SMALL_FRAME,
		TYPE_ET2_E_TIME_SMALL_FRAMELINE,
		TYPE_ET2_E_TIME_SMALL_GEARIN,
		TYPE_ET2_E_TIME_SMALL_MINUTE,
		TYPE_ET2_E_TIME_SMALL_SECOND,
		TYPE_ET2_E_TIME_SMALL_GEAR01,
		TYPE_ET2_E_TIME_SMALL_GEAR02,
		// ---------------------------
		TYPE_ET2_P_LIGHTLINE, //
		TYPE_ET2_P_CIRCLES,   //

		TYPE_ET3_EVENT_DELETE,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_TimeLab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_TimeLab(const CVfx_Engineer_Skill_TimeLab& rhs);
	virtual ~CVfx_Engineer_Skill_TimeLab() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_int m_iType = -1;
	_float3 m_fMainColor;

private:
	class CEffect* m_pBigGear_Frame  = nullptr;
	class CEffect* m_pBigGear_Minut  = nullptr;
	class CEffect* m_pBigGear_Second = nullptr;

private:
	class CEffect* m_pSmallGear_Frame     = nullptr;
	class CEffect* m_pSmallGear_FrameLine = nullptr;
	class CEffect* m_pSmallGear_GearIn    = nullptr;
	class CEffect* m_pSmallGear_Minut     = nullptr;
	class CEffect* m_pSmallGear_Second    = nullptr;
	class CEffect* m_pSmallGear_Gear01    = nullptr;
	class CEffect* m_pSmallGear_Gear02    = nullptr;

public:
	static CVfx_Engineer_Skill_TimeLab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END