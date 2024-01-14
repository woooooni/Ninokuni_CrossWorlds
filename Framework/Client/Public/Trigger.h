#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CTrigger final : public CGameObject
{
public:
	typedef struct tagTriggerDesc
	{
		TRIGGER_TYPE eTriggerType = TRIGGER_TYPE::TRIGGER_END;

		// Default
		Vec4 vStartPosition = { 0.f, 0.f, 0.f, 1.f };
		Vec3 vExtents = { 100.f, 100.f, 100.f };

		// For.Trigger ChangeBGM
		string strBGM = "";

		// For.Trigger MapName
		string strMapName = "";

	} TRIGGER_DESC;

private:
	CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo);
	virtual void Collision_Continue(const COLLISION_INFO& tInfo);
	virtual void Collision_Exit(const COLLISION_INFO& tInfo);

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_Collider();

private:
	TRIGGER_TYPE m_eTriggerType = TRIGGER_TYPE::TRIGGER_END;
	Vec3 m_vExtents = { 100.f, 100.f, 100.f };
	
private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;


private:
	string m_strBGM = "";

	

public:
	static CTrigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

