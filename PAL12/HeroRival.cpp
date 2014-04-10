#include "HeroRival.h"


HeroRival::HeroRival(Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::String& combatSceneName)
	:Rival(node, ent, combatSceneName)
{
	m_OriginPos = Rival::HERO_POSITION;
	m_OriginDirection = Ogre::Vector3::UNIT_X;

	m_IdleAnimState = ent->getAnimationState("Combat");
	m_IdleAnimState->setEnabled(true);
	m_IdleAnimState->setLoop(true);

	m_RunAnimState = ent->getAnimationState("Run");
	m_RunAnimState->setEnabled(false);
	m_RunAnimState->setLoop(true);

	m_AttackAnimState = ent->getAnimationState("Attack");
	m_AttackAnimState->setEnabled(false);
	m_AttackAnimState->setLoop(false);

	m_CurState.m_MainState = IDLE;
	m_CurState.m_SubState = IDLE;
}

HeroRival::~HeroRival(void)
{
}

void HeroRival::KillRival()
{
	//GameApplication::GetSingletonPtr()->SendMessageToScene(m_CombatSceneName, KILL_MONSTER_MSG, 0, );
}
