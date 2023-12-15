#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Model final : public CTool
{
private:
	CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void Tick_Model(_float fTimeDelta);
	void Tick_Animation(_float fTimeDelta);
	void Tick_Socket(_float fTimeDelta);
	void Tick_Event(_float fTimeDelta);
	void Tick_Costume(_float fTimeDelta);

private:
	const _bool Is_Exception();

private:
	HRESULT Ready_WeaponPrototypes();


private:
	void Reset_Transform();

private:
	wstring m_strFilePath = L"";
	wstring m_strFileName = L"";

	class CDummy* m_pDummy = nullptr;

private:

	/* Socket */

	/* 하이어러키 노드 */
	_int m_iCurBoneIndex = 0;
	
	/* 프로토타입 무기*/
	vector<class CPart*> m_WeaponPrototypes;
	_uint m_iCurWeaponIndex = 0;

	/* 계산된 소켓 행렬 컨테이너 (뼈인덱스, 트랜스폼맵) */
	map<_uint, vector<ANIM_TRANSFORM_CACHE>> m_CalculatedSockets;


public:
	static CTool_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END