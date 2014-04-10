#include "Monster.h"


Monster::Monster(Ogre::SceneNode* node, const MonsterInfo* pInfo) : m_EntityNode(node), m_pMonsterInfo(pInfo)
{
	Ogre::Entity* ent = (Ogre::Entity*)node->getAttachedObject(0);

	m_WalkAnimState = ent->getAnimationState(m_pMonsterInfo->m_AnimsName[MONSTER_ANIM_WALK]);
	m_WalkAnimState->setEnabled(false);
	m_WalkAnimState->setLoop(true);

	m_IdleAnimState = ent->getAnimationState(m_pMonsterInfo->m_AnimsName[MONSTER_ANIM_IDLE]);
	m_IdleAnimState->setEnabled(false);
	m_IdleAnimState->setLoop(true);

	m_Next = NULL;
	m_Flag = 0;

	m_CurAnimStateType = MONSTER_NONE_STATE;
}

Monster::~Monster( void )
{

}

const Ogre::Vector3& Monster::GetPosition() const
{
	return m_EntityNode->getPosition();
}

Ogre::SceneNode* Monster::GetSceneNode()
{
	return m_EntityNode;
}

bool Monster::IsNearEnough( const Ogre::Vector3& pos )
{
	const Ogre::Vector3& monsterPos = m_EntityNode->getPosition();
	return (monsterPos.x - pos.x) * (monsterPos.x - pos.x) + (monsterPos.z - pos.z) * (monsterPos.z - pos.z) < 5.0f;
}

float Monster::Distance(const Ogre::Vector3& pos)
{
	const Ogre::Vector3& monsterPos = m_EntityNode->getPosition();
	return monsterPos.distance(pos);
}

float Monster::SquareDistance(const Ogre::Vector3& pos)
{
	const Ogre::Vector3& monsterPos = m_EntityNode->getPosition();
	return monsterPos.squaredDistance(pos);
}

void Monster::MoveTo(float t, const Ogre::Vector3& destPos)
{
	if(m_CurAnimStateType != MONSTER_MOVETO_STATE)
	{
		m_CurAnimStateType = MONSTER_MOVETO_STATE;
		m_WalkAnimState->setEnabled(true);
		m_IdleAnimState->setEnabled(false);
	}

	const Ogre::Vector3& monsterPos = m_EntityNode->getPosition();

	Ogre::Vector3 direction = destPos - monsterPos;
	direction.normalise();

	float speed = m_pMonsterInfo->m_fMoveSpeed;
	m_EntityNode->translate(direction * speed * t);
	direction.y = 0;
	m_EntityNode->setDirection(-direction, Ogre::Node::TS_PARENT);
	m_WalkAnimState->addTime(t);
}

void Monster::Idle(float t)
{
	if(m_CurAnimStateType != MONSTER_IDLE_STATE)
	{
		m_CurAnimStateType = MONSTER_IDLE_STATE;
		m_IdleAnimState->setEnabled(true);
		m_WalkAnimState->setEnabled(false);
	}

	m_IdleAnimState->addTime(t);
}


void Monster::SetAnimState(MonsterStateEnum state, Ogre::AnimationState* animState)
{
	switch(state)
	{
	case MONSTER_MOVETO_STATE:
		m_WalkAnimState = animState;
		break;
	case MONSTER_IDLE_STATE:
		m_IdleAnimState = animState;
		break;
	default:
		break;
	}
}