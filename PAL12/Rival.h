#ifndef _RIVAL_CLASS_H
#define _RIVAL_CLASS_H

#include <ogre/Ogre.h>
//#include "pal.h"
#include "GameApplication.h"
#include "MonsterInfoManager.h"
#include "SceneMsgID.h"

class CombatScene;

class Rival
{
public:
	enum RivalStateEnum
	{
		ATTACK,
		IDLE,
		MOVE_TO_ME,
		MOVE_AWARY_ME,
		DEAD
	};
	struct RivalState
	{
		RivalStateEnum m_MainState;
		RivalStateEnum m_SubState;
	};
public:
	Rival(Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::String& combatSceneName);
	const Ogre::AxisAlignedBox& GetAABB();
	Ogre::SceneNode* GetSceneNode() {return m_RivalNode;}
	void SetIdle();
	void Update(float deltaTime);
	void SetState(RivalStateEnum mainState, RivalStateEnum subState) { m_CurState.m_MainState = mainState; m_CurState.m_SubState = subState;}
	void BeginAttack();
	void SetTargetRival(Rival* rival);
	inline RivalState GetCurrentState() { return m_CurState; }
	float GetHeight() {return m_fHeight;} 
	const Ogre::Vector3& GetOriginPosition() { return m_OriginPos; }
	~Rival(void);

	int GetCurrentLife() { return m_iLife; }
	void SetLife(int life) { m_iLife = life; }

	static const Ogre::Vector3 RIVAL_POSITIONS[3];
	static const Ogre::Vector3 HERO_POSITION;
protected:
	virtual void KillRival() = 0;
	Ogre::SceneNode* m_RivalNode;
	Ogre::Entity* m_Entity;
	Rival* m_pTargetRival;
	int m_iLife;
	RivalState m_CurState;
	Ogre::Vector3 m_AttackDirection;
	Ogre::AnimationState* m_IdleAnimState; 
	Ogre::AnimationState* m_RunAnimState;
	Ogre::AnimationState* m_AttackAnimState;
	Ogre::AxisAlignedBox m_Aabb;
	Ogre::Vector3 m_OriginPos;
	Ogre::Vector3 m_OriginDirection;
	Ogre::Vector3 m_TargetPos;
	float m_fHeight;
	Ogre::String m_CombatSceneName;
};

#endif

