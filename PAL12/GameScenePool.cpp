#include "GameScenePool.h"

BaseScene* GameScenePool::GetScene( Ogre::String& sceneName )
{
	for (int i = 0; i < m_SceneArray.size(); i++)
	{
		if(m_SceneArray[i]->GetName() == sceneName)
			return m_SceneArray[i];
	}

	BaseScene* scene = GameSceneFactory::CreateScene(sceneName);
	m_SceneArray.push_back(scene);
	return scene;
}

void GameScenePool::RemoveScene( Ogre::String& sceneName )
{
	std::vector<BaseScene*>::iterator it = m_SceneArray.begin();
	for (; it != m_SceneArray.end(); it++)
	{
		BaseScene* scene = *(it);
		if(scene->GetName() != sceneName)
		{
			scene->UnloadResources();
			delete scene;
			m_SceneArray.erase(it);
		}
	}
}

void GameScenePool::RemoveAllScenes()
{
	for (int i = 0; i < m_SceneArray.size(); i++)
	{
		BaseScene* scene = m_SceneArray[i];
		scene->UnloadResources();
		delete scene;
	}
	m_SceneArray.clear();
}
