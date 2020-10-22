#include "stdafx.h"
#include "GameObjectManager.h"


void GameObjectManager::Execute()
{
	for (int i = 0; i < m_gameObjectList.size(); i++) {
		m_gameObjectList[i]->Start(); 
		m_gameObjectList[i]->Update();
		m_gameObjectList[i]->Draw();
	}
}

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//すでにインスタンスを作っている
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}