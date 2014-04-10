#include "Hero.h"



Hero::Hero(Ogre::Entity* heroEntity, Ogre::SceneManager* sceneMgr, Ogre::Camera* camera, Ogre::Vector3& pos)
{
	m_RunAnimState = heroEntity->getAnimationState("Run");
	m_RunAnimState->setEnabled(false);
	m_RunAnimState->setLoop(false);

	m_WalkAnimState = heroEntity->getAnimationState("Run");
	m_WalkAnimState->setEnabled(true);
	m_WalkAnimState->setLoop(false);

	m_HeroNode = sceneMgr->createSceneNode("HeroNode");
	m_HeroNode->attachObject(heroEntity);
	m_HeroNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Math::PI));
	m_HeroNode->scale(0.5f, 0.5f, 0.5f);

	m_PivotNode = sceneMgr->getRootSceneNode()->createChildSceneNode("PivotNode");
	m_PivotNode->setPosition(pos);
	m_PivotNode->addChild(m_HeroNode);

	m_CamNode = m_PivotNode->createChildSceneNode("CamNode");
	m_CamNode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(-Ogre::Math::PI / 12));
	m_CamNode->translate(0, 0, 30.0f, Ogre::SceneNode::TS_LOCAL);
	m_CamNode->attachObject(camera);

	m_Keyboard = GameApplication::GetSingletonPtr()->GetKeyboard();
}


Hero::~Hero(void)
{
}

void Hero::Update(float deltaTime)
{

}