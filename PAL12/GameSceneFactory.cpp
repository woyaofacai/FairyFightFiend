#include "GameSceneFactory.h"

BaseScene* GameSceneFactory::CreateScene(Ogre::String& sceneName)
{
	BaseScene* scene = NULL;
	if(sceneName == "Test")
		scene = new TestScene;
	else if(sceneName == "GUITest")
		scene = new GUITestScene;
	else if(sceneName == "Combat")
		scene = new CombatScene;
	else if(sceneName == "Valley")
		scene = new ValleyScene;

	if(scene != NULL)
	{
		scene->Init(GameApplication::GetSingletonPtr()->GetRoot(), sceneName);
	}
	return scene;
}