#pragma once

#include "Client_Defines.h"
#include "DynamicObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
class CStateMachine;
END


class CDynamicProps final : public CDynamicObject
{
private:
	explicit CDynamicProps(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int iObjectType);
	explicit CDynamicProps(const CDynamicObject& rhs);
	virtual ~CDynamicProps() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

	virtual HRESULT Ready_Components();

private:
	HRESULT Ready_State();
	HRESULT Ready_AnimModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName);
public:
	static CDynamicProps* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag,
		const wstring& strFilePath, const wstring& strFileName, _int eObjType, _uint eModelType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

