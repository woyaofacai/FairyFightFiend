#include "Rival.h"


const Ogre::Vector3 Rival::RIVAL_POSITIONS[] = {
	Ogre::Vector3(-15, 0, 10),
	Ogre::Vector3(-15, 0, 0),
	Ogre::Vector3(-15, 0, -10)
};

const Ogre::Vector3 Rival::HERO_POSITION(15.0f, 0.0f, 0.0f);

Rival::Rival( Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::String& combatSceneName)
	:m_RivalNode(node), m_Entity(ent), m_CombatSceneName(combatSceneName)
{
	m_CurState.m_MainState = IDLE;
	m_CurState.m_SubState = IDLE;
	//m_OriginDirection = Ogre::Vector3::NEGATIVE_UNIT_X;

	m_fHeight = 0;
}

Rival::~Rival(void)
{

}

void Rival::SetIdle()
{
	m_CurState.m_MainState = IDLE;
	m_CurState.m_SubState = IDLE;
	m_IdleAnimState->setEnabled(true);
	m_AttackAnimState->setEnabled(false);
	m_RunAnimState->setEnabled(false);
}

void Rival::Update(float deltaTime)
{
	if(m_CurState.m_MainState == IDLE)
		m_IdleAnimState->addTime(deltaTime);
	else if(m_CurState.m_MainState == ATTACK)
	{
		if(m_CurState.m_SubState == MOVE_TO_ME)
		{
			Ogre::Vector3 pos = m_RivalNode->_getDerivedPosition();
			Ogre::Vector3 vecToTarget = m_TargetPos - pos;
			if(fabs(vecToTarget.x) < 3.0f)
			{
				m_CurState.m_SubState = ATTACK;
				m_RunAnimState->setEnabled(false);
				m_AttackAnimState->setEnabled(true);
				m_AttackAnimState->setTimePosition(0.0f);
			}
			else {
				m_RivalNode->translate(0, 0, deltaTime * 20.0f, Ogre::Node::TS_LOCAL);
				m_RunAnimState->addTime(deltaTime * 2);
			}
		}
		else if(m_CurState.m_SubState == ATTACK)
		{
			if(m_AttackAnimState->hasEnded())
			{
				m_AttackAnimState->setTimePosition(0.0f);
				m_AttackAnimState->setEnabled(false);
				m_RunAnimState->setEnabled(true);
				m_RivalNode->yaw(Ogre::Radian(Ogre::Math::PI), Ogre::Node::TS_LOCAL);
				m_CurState.m_SubState = MOVE_AWARY_ME;


				int curMonsLife = m_pTargetRival->GetCurrentLife();
				int lifeDecrease = 20 + rand() % 10;
				m_pTargetRival->SetLife(curMonsLife - lifeDecrease);
					
				std::cout << "Life: " << m_pTargetRival->GetCurrentLife() << std::endl;

				if(m_pTargetRival->GetCurrentLife() <= 0)
				{
					GameApplication::GetSingletonPtr()->SendMessageToScene(m_CombatSceneName, KILL_RIVAL_MSG, 0, m_pTargetRival);
					//Ogre::AnimationState* dieAnim = m_Entity->getAnimationState("");
				}
			}
			else {
				m_AttackAnimState->addTime(deltaTime);
			}
		}
		else {
			const Ogre::Vector3& pos = m_RivalNode->_getDerivedPosition();
			Ogre::Vector3 vecFromRivalToHero = m_OriginPos - pos;

			if(fabs(vecFromRivalToHero.x) < 1.0f)
			{
				m_CurState.m_MainState = m_CurState.m_SubState = IDLE;
				m_RunAnimState->setEnabled(false);
				m_IdleAnimState->setEnabled(true);
				m_RivalNode->setDirection(m_OriginDirection, Ogre::Node::TS_PARENT);
				m_RivalNode->setPosition(m_OriginPos);
			}
			else {
				m_RivalNode->translate(0, 0, deltaTime * 20.0f, Ogre::Node::TS_LOCAL);
				m_RunAnimState->addTime(deltaTime * 2);
			}
		}
		
	}
}

void Rival::BeginAttack()
{
	m_CurState.m_MainState = ATTACK;
	m_CurState.m_SubState = MOVE_TO_ME;
	m_IdleAnimState->setEnabled(false);
	m_AttackAnimState->setEnabled(false);
	m_RunAnimState->setEnabled(true);
	m_RivalNode->setDirection(-m_AttackDirection, Ogre::Node::TS_PARENT);
}

const Ogre::AxisAlignedBox& Rival::GetAABB()
{
	return m_Aabb;
}

void Rival::SetTargetRival(Rival* rival)
{
	m_pTargetRival = rival;
	m_TargetPos = m_pTargetRival->GetOriginPosition();
	m_AttackDirection = m_TargetPos - m_OriginPos;
	m_AttackDirection.normalise();
}
