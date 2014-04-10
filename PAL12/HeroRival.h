#ifndef _HERO_RIVAL_H
#define _HERO_RIVAL_H

#include "Rival.h"

class HeroRival : public Rival
{
public:
	HeroRival(Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::String& combatSceneName);
	~HeroRival(void);
	virtual void KillRival();
protected:
	Rival* m_pTargetRival;
};

#endif

