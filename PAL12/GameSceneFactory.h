#ifndef _GAMESCENEFACTORY_H
#define _GAMESCENEFACTORY_H

#include "GameApplication.h"
#include "GameScene.h"
#include "TestScene.h"
#include "GUITestScene.h"
#include "CombatScene.h"
#include "ValleyScene.h"

class GameSceneFactory
{
public:
	static BaseScene* CreateScene(Ogre::String& sceneName);
};

#endif