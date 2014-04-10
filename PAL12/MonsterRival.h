#ifndef _MONSTER_RIVAL_H
#define _MONSTER_RIVAL_H

#include "Rival.h"

class MonsterRival : public Rival
{
public:
	MonsterRival(Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::String& combatSceneName, const MonsterInfo* pMonsterInfo, int posNum);
	~MonsterRival(void);
protected:
	virtual void KillRival();
private:
	int m_iPosNum;
	const MonsterInfo* m_pMonsterInfo;	
};

#endif

