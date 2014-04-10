#ifndef _GAMESCENEPOOL_H
#define _GAMESCENEPOOL_H

#include "GameSceneFactory.h"

class GameScenePool
{
public:
	static GameScenePool& GetSingleton()
	{
		static GameScenePool pool;
		return pool;
	}

	BaseScene* GetScene(Ogre::String& sceneName);

	void RemoveScene(Ogre::String& sceneName);

	void RemoveAllScenes();

	~GameScenePool(void) {}
private:
	GameScenePool(void) {}
	std::vector<BaseScene*> m_SceneArray;
};

#endif