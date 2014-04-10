#include "MonsterRival.h"
#include "GameApplication.h"

MonsterRival::MonsterRival(Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::String& combatSceneName, const MonsterInfo* pMonsterInfo, int posNum)
	:Rival(node, ent, combatSceneName), m_pMonsterInfo(pMonsterInfo)
{
	m_iPosNum = posNum;
	m_OriginPos = Rival::RIVAL_POSITIONS[posNum];
	m_TargetPos = Rival::HERO_POSITION;
	m_OriginDirection = Ogre::Vector3::NEGATIVE_UNIT_X;

	m_AttackDirection = m_TargetPos - m_OriginPos;
	m_AttackDirection.normalise();

	m_IdleAnimState = ent->getAnimationState(pMonsterInfo->m_AnimsName[MONSTER_ANIM_IDLE]);
	m_IdleAnimState->setEnabled(true);
	m_IdleAnimState->setLoop(true);

	m_RunAnimState = ent->getAnimationState(pMonsterInfo->m_AnimsName[MONSTER_ANIM_RUN]);
	m_RunAnimState->setEnabled(false);
	m_RunAnimState->setLoop(true);

	m_AttackAnimState = ent->getAnimationState(pMonsterInfo->m_AnimsName[MONSTER_ANIM_ATTACK]);
	m_AttackAnimState->setEnabled(false);
	m_AttackAnimState->setLoop(false);

	m_CurState.m_MainState = IDLE;
	m_CurState.m_SubState = IDLE;

	m_Aabb = ent->getBoundingBox();
	m_fHeight = (m_Aabb.getMaximum().y - m_Aabb.getMinimum().y) * pMonsterInfo->m_fScale;

	if(m_pMonsterInfo->m_iMinLife == m_pMonsterInfo->m_iMaxLife)
		m_iLife = m_pMonsterInfo->m_iMinLife;
	else 
		m_iLife = m_pMonsterInfo->m_iMinLife + rand() % (m_pMonsterInfo->m_iMaxLife - m_pMonsterInfo->m_iMinLife);
}


MonsterRival::~MonsterRival(void)
{

}

void MonsterRival::KillRival()
{
	//GameApplication::GetSingletonPtr()->SendMessageToScene(m_CombatSceneName, KILL_HERO_MSG, 0, NULL);
}
