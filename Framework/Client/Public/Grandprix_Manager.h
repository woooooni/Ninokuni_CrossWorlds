#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
END

BEGIN(Client)
class CGrandprix_Manager : public CBase
{
	DECLARE_SINGLETON(CGrandprix_Manager)

private:
	CGrandprix_Manager();
	virtual ~CGrandprix_Manager() = default;

public:
	enum GRANDPRIX_PHASE { GRANDPRIX_NO_RUN, GRANDPRIX_PREPARE, GRANDPRIX_PROGRESS, GRANDPRIX_FINISH, GRANDPRIX_END, PHASE_END };

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);
	void	LateTick(_float fTimeDelta);

public:
	HRESULT Ready_Grandprix_GameObjectToLayer(LEVELID eID);
	void Ready_Grandprix_EnemyInfo();

public:
	HRESULT Prepare_Grandprix();
	void Start_Grandprix();
	void Finish_Grandprix();
	void End_Grandprix();

public:
	void Add_ItemBox(_uint iType);
	void Show_GoalObject();

private:
	GRANDPRIX_PHASE m_eCurPhase = { GRANDPRIX_PHASE::GRANDPRIX_NO_RUN };

	class CGrandprix_Engineer* m_pEngineer = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEnemyPlane = { nullptr };
	vector <class CVehicle_Flying_EnemyBoto*> m_Botos;
	vector <class CGrandprix_Enemy*> m_Enemies;
	vector <class CGrandprix_ItemBox*> m_Items;

	class CGrandprix_Goal* m_pGoal = { nullptr };

	_bool m_bItem[4];
	_float m_fTimeAcc[4];

private:
	void Ready_Riders();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};
END