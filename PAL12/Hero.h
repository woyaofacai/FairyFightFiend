#ifndef _HERO_CLASS_H
#define _HERO_CLASS_H

#include "Ogre\Ogre.h"
#include "OIS\OIS.h"
#include "GameApplication.h"

class Hero
{
public:
	Hero(Ogre::Entity* heroEntity, Ogre::SceneManager* sceneMgr, Ogre::Camera* camera, Ogre::Vector3& pos);
	~Hero(void);
	void Update(float deltaTime);
private:
	Ogre::SceneNode* m_PivotNode;
	Ogre::SceneNode* m_HeroNode;
	Ogre::SceneNode* m_CamNode;
	Ogre::AnimationState* m_RunAnimState;
	Ogre::AnimationState* m_WalkAnimState;

	OIS::Keyboard* m_Keyboard;
};

#endif

