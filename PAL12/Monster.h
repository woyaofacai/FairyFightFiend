#ifndef _MONSTER_H
#define _MONSTER_H

#include <ogre/Ogre.h>
#include "MonsterInfoManager.h"

struct MonsterTile
{
	int row;
	int col;
};

enum MonsterStateEnum
{
	MONSTER_NONE_STATE = 0,
	MONSTER_MOVETO_STATE = 1,
	MONSTER_IDLE_STATE = 2
};

class Monster
{
public:
	Monster(Ogre::SceneNode* node, const MonsterInfo* pInfo);
	~Monster(void);
	const Ogre::Vector3& GetPosition() const;
	inline unsigned int GetTypeID() { return m_pMonsterInfo->m_Id; }
	Ogre::SceneNode* GetSceneNode();
	bool IsNearEnough(const Ogre::Vector3& pos);
	float Distance(const Ogre::Vector3& pos);
	float SquareDistance(const Ogre::Vector3& pos);
	void MoveTo(float t, const Ogre::Vector3& destPos);
	void SetAnimState(MonsterStateEnum state, Ogre::AnimationState* animState);
	void Idle(float t);
	Monster* m_Next;
	int m_Flag;
	MonsterStateEnum m_CurAnimStateType;
private:
	const MonsterInfo* m_pMonsterInfo;
	Ogre::SceneNode* m_EntityNode;
	Ogre::AnimationState* m_WalkAnimState;
	Ogre::AnimationState* m_IdleAnimState;
	int m_iRivalsNum;
};

#endif