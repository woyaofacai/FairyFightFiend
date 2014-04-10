#include "ValleyScene.h"
#include "GameApplication.h"

void ValleyScene::CreateSceneManager()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

void ValleyScene::CreateScene()
{
	MazeBaseScene::CreateScene();

	std::string terrain_cfg("hill.cfg");
	mSceneMgr->setWorldGeometry( terrain_cfg );

	m_BackMusic = SoundManager::GetSingleton().GetSound("pal3.wav", true);
}

bool ValleyScene::IsHeroPositionValid()
{
	const Ogre::Vector3& pivotPos = m_PivotNode->getPosition();

	Ogre::Ray ray;
	ray.setDirection(-Ogre::Vector3::UNIT_Y);
	ray.setOrigin(Ogre::Vector3(pivotPos.x, pivotPos.y + 10, pivotPos.z));
	m_pRaySceneQuery->setRay(ray);

	Ogre::RaySceneQueryResult& result = m_pRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator resultIter = result.begin();

	bool bValid = true;
	if(resultIter != result.end() && resultIter->worldFragment)
	{
		if(resultIter->worldFragment->singleIntersection.y > 5.0f)
		{
			bValid = false;
		}
		else {
			m_PivotNode->setPosition(pivotPos.x, resultIter->worldFragment->singleIntersection.y, pivotPos.z);
		}
	}

	return bValid;
}

bool ValleyScene::frameStarted(const Ogre::FrameEvent& evt)
{
	MazeBaseScene::frameStarted(evt);

	float forwardDist = 10.0f;
	m_CamNode->setPosition(0, 7, 0);
	m_CamNode->translate(0, 0, 30, Ogre::Node::TS_LOCAL);

	bool flag = false;
	while(!flag)
	{
		Ogre::Ray ray;
		const Ogre::Vector3& camPos = m_CamNode->_getDerivedPosition();
		ray.setOrigin(Ogre::Vector3(camPos.x, 200, camPos.z));
		ray.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
		m_pRaySceneQuery->setRay(ray);
		Ogre::RaySceneQueryResult& result = m_pRaySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator resultIter = result.begin();

		if(resultIter != result.end() && resultIter->worldFragment)
		{
			if(resultIter->worldFragment->singleIntersection.y > 10.0f && forwardDist > 1.0f)
			{
				m_CamNode->translate(0, 0, -forwardDist, Ogre::Node::TS_LOCAL);
				forwardDist *= 0.5f;
			}
			else 
				flag = true;
		}
		else 
			flag = true;
	}

	return true;
}

void ValleyScene::SetActive()
{
	m_BackMusic->setIsPaused(false);
}

void ValleyScene::SetUnactive()
{
	m_BackMusic->setIsPaused(true);
}
