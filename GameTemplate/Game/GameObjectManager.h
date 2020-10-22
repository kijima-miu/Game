#pragma once
#include <vector>
#include "IGameObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	static GameObjectManager* GetInstance()
	{
		return m_instance;
	}

	///<summary>
	///�Q�[���I�u�W�F�N�g��o�^����B
	///</summary>
	///<param name="go">�o�^����Q�[���I�u�W�F�N�g</param>
	void AddGameObjectManager(IGameObject* go)
	{
		m_gameObjectList.push_back(go);
	}

	///<summary>
	///�Q�[���I�u�W�F�N�g���폜����B
	///</summary>
	///<param name="go">�폜����Q�[���I�u�W�F�N�g</param>
	void DeleteGameObjectManager(IGameObject* go)
	{
		/*m_gameObjectList.erase(std::remove(
			m_gameObjectList.begin(),
			m_gameObjectList.end(),
			go),
			m_gameObjectList.end()
		);*/
		std::vector<IGameObject*>::iterator st = m_gameObjectList.begin();
		int en = m_gameObjectList.size();
		for (int i = 0; i < en; i++) {
			if (*st == go) {
				//go�̗v�f���������č폜
				m_gameObjectList.erase(st);
				break;
			}
			st++;
		}
	}

	///<summary>
	///Start�֐��AUpdate�֐��ADraw�֐����Ăяo���B
	///</summary>
	void Execute();
private:
	std::vector<IGameObject*> m_gameObjectList;
	static GameObjectManager* m_instance;
};

