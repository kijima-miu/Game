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
	///ゲームオブジェクトを登録する。
	///</summary>
	///<param name="go">登録するゲームオブジェクト</param>
	void AddGameObjectManager(IGameObject* go)
	{
		m_gameObjectList.push_back(go);
	}

	///<summary>
	///ゲームオブジェクトを削除する。
	///</summary>
	///<param name="go">削除するゲームオブジェクト</param>
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
				//goの要素を検索して削除
				m_gameObjectList.erase(st);
				break;
			}
			st++;
		}
	}

	///<summary>
	///Start関数、Update関数、Draw関数を呼び出す。
	///</summary>
	void Execute();
private:
	std::vector<IGameObject*> m_gameObjectList;
	static GameObjectManager* m_instance;
};

