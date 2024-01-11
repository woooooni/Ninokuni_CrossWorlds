#include "..\Public\Collision_Manager.h"
#include "GameObject.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollision_Manager)


CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Reserve_Manager()
{
	Reset();
	Reserve_CheckGroup(COLLISION_GROUP::CHARACTER, COLLISION_GROUP::MONSTER);
	Reserve_CheckGroup(COLLISION_GROUP::CHARACTER, COLLISION_GROUP::TRIGGER);
	Reserve_CheckGroup(COLLISION_GROUP::CHARACTER, COLLISION_GROUP::ANIMAL);

	for (_uint i = 0; i < COLLISION_GROUP::GROUP_END; ++i)
		m_CollisionObjects[i].reserve(300);



	return S_OK;
}

HRESULT CCollision_Manager::Reserve_CheckGroup(COLLISION_GROUP eLeft, COLLISION_GROUP eRight)
{
	_uint iRow = (_uint)eLeft;
	_uint iCol = (_uint)eRight;

	if (iCol < iRow)
	{
		iRow = (_uint)eRight;
		iCol = (_uint)eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}

	return S_OK;
}

void CCollision_Manager::LateTick(_float fTimeDelta)
{
	for (_uint iRow = 0; iRow < (_uint)COLLISION_GROUP::GROUP_END; iRow++)
	{
		for (UINT iCol = iRow; iCol < (UINT)COLLISION_GROUP::GROUP_END; iCol++)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				Collision_Update((COLLISION_GROUP)iRow, (COLLISION_GROUP)iCol);
			}
		}
	}

	for (_uint i = 0; i < (_uint)COLLISION_GROUP::GROUP_END; ++i)
		m_CollisionObjects[i].clear();
}

HRESULT CCollision_Manager::Add_CollisionGroup(COLLISION_GROUP eGroup, CGameObject* pGameObject)
{
	m_CollisionObjects[eGroup].push_back(pGameObject);
	return S_OK;
}




void CCollision_Manager::Collision_Update(COLLISION_GROUP eLeft, COLLISION_GROUP eRight)
{
	const vector<CGameObject*>& pLeftObject = m_CollisionObjects[eLeft];
	const vector<CGameObject*>& pRightObject = m_CollisionObjects[eRight];

	map<ULONGLONG, bool>::iterator iter;

	for (auto& pLeft : pLeftObject)
	{
		for (auto& pRight : pRightObject)
		{
			if (pLeft == pRight)
				continue;

			for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
			{
				vector<CCollider*>& LeftColliders = pLeft->Get_Collider(i);

				for (_uint j = 0; j < CCollider::DETECTION_TYPE::DETECTION_END; ++j)
				{
					vector<CCollider*>& RightColliders = pRight->Get_Collider(j);

					for (auto& pLeftCollider : LeftColliders)
					{
						for (auto& pRightCollider : RightColliders)
						{

							

							COLLIDER_ID ID;
							ID.iLeft_id = min(pLeftCollider->Get_ColliderID(), pRightCollider->Get_ColliderID());
							ID.iRight_id = max(pLeftCollider->Get_ColliderID(), pRightCollider->Get_ColliderID());

							iter = m_mapColInfo.find(ID.ID);
							if (iter == m_mapColInfo.end())
							{
								if (false == pLeftCollider->Is_Active() && false == pRightCollider->Is_Active())
									continue;

								m_mapColInfo.insert(make_pair(ID.ID, false));
								iter = m_mapColInfo.find(ID.ID);
							}
							
							if (Is_Collision(pLeftCollider, pRightCollider))
							{
								if (iter->second)
								{
									if (pLeft->Is_Dead() || pRight->Is_Dead())
									{
										pLeftCollider->Collision_Exit(pRightCollider);
										pRightCollider->Collision_Exit(pLeftCollider);
										iter->second = false;
									}
									else
									{
										pLeftCollider->Collision_Continue(pRightCollider);
										pRightCollider->Collision_Continue(pLeftCollider);
									}
								}
								else
								{
									if (false == pLeft->Is_Dead() && false == pRight->Is_Dead())
									{
										pLeftCollider->Collision_Enter(pRightCollider);
										pRightCollider->Collision_Enter(pLeftCollider);
										iter->second = true;
									}
								}
							}
							else
							{
								if (iter->second)
								{
									pLeftCollider->Collision_Exit(pRightCollider);
									pRightCollider->Collision_Exit(pLeftCollider);
									iter->second = false;
								}
							}

						}
					}

					
				}
			}
		}
	}


	
}

_bool CCollision_Manager::Is_Collision(CCollider* pLeft, CCollider* pRight)
{
	if (false == pLeft->Is_Active() || false == pRight->Is_Active())
		return false;

	return pLeft->Is_Collision(pRight);
}

void CCollision_Manager::Free()
{
	__super::Free();
	/*for (_uint i = 0; i < (_uint)COLLISION_GROUP::GROUP_END; ++i)
	{
		for (auto& pObject : m_CollisionObjects[(_uint)i])
			Safe_Release(pObject);

		m_CollisionObjects[(_uint)i].clear();
	}*/
	
}
