#pragma once
#include "Character.h"

BEGIN(Client)
class CUI_CharacterDummy final : public CCharacter
{

private:
	CUI_CharacterDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CUI_CharacterDummy(const CUI_CharacterDummy& rhs);
	virtual ~CUI_CharacterDummy() = default;

public:
	_bool Get_Active() { return m_bActive; }
	void Set_Active(_bool bActive) {
		m_bActive = bActive;

		if (bActive)
			m_pTransformCom->LookAt_ForLandObject(m_vCamPosition);
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	_bool m_bActive = { false };
	_vector m_vCamPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_matrix m_vCamMatrix = XMMatrixIdentity();
	_float4x4 m_ViewMatrix;
	CHARACTER_TYPE m_eCurCharacter = { CHARACTER_TYPE::SWORD_MAN };

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

private:
	_float2 Transpose_ProjectionPosition();
	
public:
	static CUI_CharacterDummy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

