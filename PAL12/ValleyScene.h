#ifndef _VALLEY_SCENE_H
#define _VALLEY_SCENE_H

#include "MazeBaseScene.h"
#include "SoundManager.h"

class ValleyScene : public MazeBaseScene
{
public:
	ValleyScene()
	{
		m_BackgroundColor = Ogre::ColourValue(0.93f, 0.86f, 0.76f);
	}
	virtual void CreateScene();
	virtual bool IsHeroPositionValid();
	virtual void CreateSceneManager();
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual void SetActive();
	virtual void SetUnactive();
private:
	irrklang::ISound* m_BackMusic;
};

#endif

