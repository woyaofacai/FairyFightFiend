#ifndef _GAMEAPPLICATION_H
#define _GAMEAPPLICATION_H

#include <ogre/Ogre.h>
#include <ois/OIS.h>

class GameApplication
{
public:
	static GameApplication* GetSingletonPtr();
	static void SetSingleton(GameApplication* pSingleton);
	virtual void StartUp() = 0;
	virtual Ogre::Root* GetRoot() const = 0;
	virtual Ogre::RenderWindow* GetWindow() const = 0; 
	virtual OIS::Mouse* GetMouse() const = 0;
	virtual OIS::Keyboard* GetKeyboard() const = 0;
	virtual bool ReplaceScene(Ogre::String& targetSceneName, bool unloadPreSceneResource = true, bool loadTargetSceneSource = true) = 0;
	virtual void CreateScene(Ogre::String& sceneName, bool bLoadResource) = 0;
	virtual void SendMessageToScene(Ogre::String& targetSceneName, Ogre::uint32 id, Ogre::int32 data, void* lpinfo) = 0;
private:
	static GameApplication* m_pSingleton;
}; 

#endif