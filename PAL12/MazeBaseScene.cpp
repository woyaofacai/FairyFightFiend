#include "MazeBaseScene.h"
#include "CombatScene.h"
#include "GameApplication.h"
#include "GameScenePool.h"

#define HEROINE_WALK_SPEED 20.0f
#define HEROINE_RUN_SPEED 40.0f

struct MonsterPositionInfo
{
	unsigned int id;
	float pos[3];
}; 

struct MazeInfoHeader
{
	float m_fMazeWidth;
	float m_fMazeDepth;
	float m_fStartPosition[3];
	float m_fRivalNumProbabilities[3];
	int m_iMonsterTileCol;
	int m_iMonsterTileRow;
	int m_iNumMonsterTypes;
	int m_iMonstersTypeOffset;
	int m_iNumMonsters;
	int m_iMonsterOffset;
};


MazeBaseScene::MazeBaseScene()
{
	m_bReturnMenu = false;
}


bool MazeBaseScene::keyPressed( const OIS::KeyEvent &arg )
{
	if(arg.key == OIS::KC_ESCAPE)
		m_bReturnMenu = true;
	return true;
}

void MazeBaseScene::CreateScene()
{
	char szMazeConfigFile[100];
	sprintf(szMazeConfigFile, "../media/maze/%s.maze", mSceneName.c_str());
	FILE* fp = fopen(szMazeConfigFile, "rb");

	int iStart = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int iEnd = ftell(fp);
	int iFileSize = iEnd - iStart;
	fseek(fp, 0, SEEK_SET);

	unsigned char* pFileBegin = (unsigned char*)malloc(iFileSize);
	unsigned char* pCurrent;
	fread((void*)pFileBegin, iFileSize, 1, fp);
	fclose(fp);

	MazeInfoHeader* pHeader = (MazeInfoHeader*)pFileBegin;

	memcpy((void*)m_fRivalNumProbabilities, (void*)pHeader->m_fRivalNumProbabilities, sizeof(float) * 3);
	pCurrent = pFileBegin + pHeader->m_iMonstersTypeOffset;

	m_iNumMonsterType = pHeader->m_iNumMonsterTypes;
	m_MonsterTypeAndProbs = new MonsterTypeAndProbability[m_iNumMonsterType];
	memcpy((void*)m_MonsterTypeAndProbs, (void*)pCurrent, m_iNumMonsterType * sizeof(MonsterTypeAndProbability));

	m_iNumMonsterTileRow = pHeader->m_iMonsterTileRow;
	m_iNumMonsterTileCol = pHeader->m_iMonsterTileCol;
	m_iNumMonsterTile = m_iNumMonsterTileRow * m_iNumMonsterTileCol;

	m_fMazeWidth = pHeader->m_fMazeWidth;
	m_fMazeDepth = pHeader->m_fMazeDepth;
	
	m_fMonsterTileWidth = m_fMazeWidth / m_iNumMonsterTileCol;
	m_fMonsterTileDepth = m_fMazeDepth / m_iNumMonsterTileRow;

	m_MonsterTiles = new Monster*[m_iNumMonsterTile];

	pCurrent = pFileBegin + pHeader->m_iMonsterOffset;

	memset((void*)m_MonsterTiles, 0, m_iNumMonsterTile * sizeof(Monster*));

	Ogre::SceneNode* monstersRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	for (int i = 0; i < pHeader->m_iNumMonsters; i++)
	{
		MonsterPositionInfo* pMonsterPositionInfo = (MonsterPositionInfo*)pCurrent;
		MonsterInfo* pMonsterInfo = MonsterInfoManager::GetSingleton().FindMonsterInfo(pMonsterPositionInfo->id);
		
		Ogre::Entity* monsterEntity = mSceneMgr->createEntity(Ogre::String(pMonsterInfo->m_MeshName));
		monsterEntity->setRenderQueueGroup(10);
		Ogre::SceneNode* monsterNode = mSceneMgr->createSceneNode();
		monsterNode->attachObject(monsterEntity);
		monsterNode->scale(pMonsterInfo->m_fScale, pMonsterInfo->m_fScale, pMonsterInfo->m_fScale);
		monstersRootNode->addChild(monsterNode);
		monsterNode->setPosition(pMonsterPositionInfo->pos[0], pMonsterPositionInfo->pos[1], pMonsterPositionInfo->pos[2]);
		Monster* monster = new Monster(monsterNode, pMonsterInfo);
		int tileIndex = _GetMonsterTile(pMonsterPositionInfo->pos[0], pMonsterPositionInfo->pos[2]);
		monster->m_Next = m_MonsterTiles[tileIndex];
		m_MonsterTiles[tileIndex] = monster;

		pCurrent += sizeof(MonsterPositionInfo);
	}

	Ogre::Entity* heroEntity = mSceneMgr->createEntity("Heroine", "zidane.mesh");

	Ogre::AnimationStateSet* animSet = heroEntity->getAllAnimationStates();
	Ogre::AnimationStateIterator it = animSet->getAnimationStateIterator();
	while(it.hasMoreElements())
	{
		std::cout << it.getNext()->getAnimationName() << std::endl;
	}


	m_RunAnimState = heroEntity->getAnimationState("Run");
	m_RunAnimState->setEnabled(false);
	m_RunAnimState->setLoop(false);

	m_WalkAnimState = heroEntity->getAnimationState("Run");
	m_WalkAnimState->setEnabled(true);
	m_WalkAnimState->setLoop(false);

	Ogre::SceneNode* heroNode = mSceneMgr->createSceneNode("HeroNode");
	heroNode->attachObject(heroEntity);
	heroNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Math::PI));
	heroNode->scale(0.5f, 0.5f, 0.5f);

	m_PivotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PivotNode");
	m_PivotNode->setPosition(pHeader->m_fStartPosition[0], pHeader->m_fStartPosition[1], pHeader->m_fStartPosition[2]);
	m_PivotNode->addChild(heroNode);

	m_CamNode = m_PivotNode->createChildSceneNode("CamNode");
	m_CamNode->setPosition(0, 7, 0);
	m_CamNode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(-Ogre::Math::PI / 12));
	m_CamNode->translate(0, 0, 30.0f, Ogre::SceneNode::TS_LOCAL);
	m_CamNode->attachObject(mCamera);

	m_pRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
}

int MazeBaseScene::_GetMonsterTile( float x, float z, int* pCol /*= NULL*/, int* pRow /*= NULL*/ )
{
	int col = x / m_fMazeWidth * m_iNumMonsterTileCol;
	int row = z / m_fMazeDepth * m_iNumMonsterTileRow;

	//std::cout << "col: " << col << " row: " << row <<std::endl;

	if(pCol != NULL) *pCol = col;
	if(pRow != NULL) *pRow = row;
	return row * m_iNumMonsterTileCol + col;
}

bool MazeBaseScene::frameStarted( const Ogre::FrameEvent& evt )
{
	if(m_bReturnMenu)
	{
		m_bReturnMenu = false;
		GameApplication::GetSingletonPtr()->ReplaceScene(Ogre::String("GUITest"), false, false);
		return true;
	}

	bool bWalked = false;
	bool bRun = false;

	Ogre::Vector3 trans(0, 0, 0);
	float fRot = 0.0f;
	float fMovement = 0.0f;

	if(mKeyboard->isKeyDown(OIS::KC_UP))
	{
		trans.z -= 1.0f;
		if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			bRun = true;
		else 
			bWalked = true;
	}
	else if(mKeyboard->isKeyDown(OIS::KC_DOWN))
	{
		trans.z += 1.0f;
		if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			bRun = true;
		else 
			bWalked = true;
	}

	if(mKeyboard->isKeyDown(OIS::KC_LEFT))
	{
		fRot -= 1.0f;
	}
	else if(mKeyboard->isKeyDown(OIS::KC_RIGHT))
	{
		fRot += 1.0f;
	}

	m_PivotNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(fRot * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);

	float prePivotHeight = m_PivotNode->getPosition().y;

	if(bWalked)
	{
		m_RunAnimState->setEnabled(false);
		m_WalkAnimState->setEnabled(true);

		fMovement = evt.timeSinceLastFrame * HEROINE_WALK_SPEED;
		m_PivotNode->translate(trans * fMovement, Ogre::Node::TS_LOCAL);
		m_WalkAnimState->addTime(evt.timeSinceLastFrame * 0.8f);
		if(m_WalkAnimState->hasEnded())
		{
			m_WalkAnimState->setTimePosition(0.0f);
		}
	}
	else if(bRun) {
		m_WalkAnimState->setEnabled(false);
		m_RunAnimState->setEnabled(true);
		fMovement = evt.timeSinceLastFrame * HEROINE_RUN_SPEED;
		m_PivotNode->translate(trans * fMovement, Ogre::Node::TS_LOCAL);
		m_RunAnimState->addTime(evt.timeSinceLastFrame);
		if(m_RunAnimState->hasEnded())
		{
			m_RunAnimState->setTimePosition(0.0f);
		}
	}


	if(bWalked || bRun)
	{
		if(!IsHeroPositionValid())
		{
			m_PivotNode->translate(-trans * fMovement, Ogre::Node::TS_LOCAL);
		}
	}

	_UpdateMonsters(evt.timeSinceLastFrame);

	return true;
}

void MazeBaseScene::_UpdateMonsters(float deltaTime)
{
	Ogre::Vector3 pivotPos = m_PivotNode->_getDerivedPosition();
	int col, row;
	int tileIndex = _GetMonsterTile(pivotPos.x, pivotPos.z, &col, &row);

	float tileStartX = col * m_fMonsterTileWidth;
	float tileStartZ = row * m_fMonsterTileWidth;

	MonsterTile indexes[4];

	indexes[0].col = col;
	indexes[0].row = row;

	if(pivotPos.x - tileStartX < m_fMonsterTileWidth * 0.5f)
	{
		indexes[1].col = col - 1;
		indexes[1].row = row;
		if(pivotPos.z - tileStartZ < m_fMonsterTileWidth * 0.5f)
		{
			indexes[2].col = col;
			indexes[2].row = row - 1;
			indexes[3].col = col - 1;
			indexes[3].row = row - 1;
		}
		else {
			indexes[2].col = col;
			indexes[2].row = row + 1;
			indexes[3].col = col - 1;
			indexes[3].row = row + 1;
		}
	}
	else {
		indexes[1].col = col + 1;
		indexes[1].row = row;
		if(pivotPos.z - tileStartZ < m_fMonsterTileWidth * 0.5f)
		{
			indexes[2].col = col;
			indexes[2].row = row - 1;
			indexes[3].col = col + 1;
			indexes[3].row = row - 1;
		}
		else {
			indexes[2].col = col;
			indexes[2].row = row + 1;
			indexes[3].col = col + 1;
			indexes[3].row = row + 1;
		}
	}


	for (int i = 0; i < 4; i++)
	{
		if(indexes[i].col < 0 || indexes[i].col >= m_iNumMonsterTileCol
			|| indexes[i].row < 0 || indexes[i].row >= m_iNumMonsterTileRow)
			continue;

		tileIndex = indexes[i].row * m_iNumMonsterTileCol + indexes[i].col;

		Monster* monster = m_MonsterTiles[tileIndex];
		Monster* pre = NULL;

		while(monster)
		{
			float dist = monster->SquareDistance(pivotPos);
			if(dist < 5.0f)
			{
				if(pre == NULL)
				{
					m_MonsterTiles[tileIndex] = m_MonsterTiles[tileIndex]->m_Next;
				}
				else {
					pre->m_Next = monster->m_Next;
				}

				mSceneMgr->destroySceneNode(monster->GetSceneNode());
				unsigned int monsterOnBattle[3];
				int iNumMonsterOnBattle;
				iNumMonsterOnBattle = _GenerateMonsterGroupOnBattle(monster->GetTypeID(), monsterOnBattle);

				delete monster;
				
				CombatScene* combatScene = (CombatScene*)GameScenePool::GetSingleton().GetScene(Ogre::String("Combat"));
				combatScene->m_iMonsterNum = iNumMonsterOnBattle;
				memcpy((void*)combatScene->m_MonsterTypes, monsterOnBattle, sizeof(unsigned int) * 3);
				GameApplication::GetSingletonPtr()->ReplaceScene(Ogre::String("Combat"), false, true);
				return;
			}
			else if(dist < 2500.0f) 
			{
				//mCamera->getCullingFrustum()->
				Ogre::Frustum* frumstum = mCamera->getCullingFrustum();

				if(mCamera->isVisible(monster->GetSceneNode()->_getWorldAABB()))
				{
					monster->MoveTo(deltaTime, pivotPos);
					monster->m_Flag = 1;
				}
			}
			else if(dist < 10000.0f)
			{
				if(mCamera->isVisible(monster->GetSceneNode()->_getWorldAABB()))
				{
					monster->Idle(deltaTime);
				}
			}
			pre = monster;
			monster = monster->m_Next;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if(indexes[i].col < 0 || indexes[i].col >= m_iNumMonsterTileCol
			|| indexes[i].row < 0 || indexes[i].row >= m_iNumMonsterTileRow)
			continue;

		tileIndex = row * m_iNumMonsterTileCol + col;

		Monster* monster = m_MonsterTiles[tileIndex];
		Monster* pre = NULL;

		while(monster)
		{
			if(monster->m_Flag == 1)
			{
				const Ogre::Vector3& monsterPos = monster->GetPosition();
				int newTileIndex = _GetMonsterTile(monsterPos.x, monsterPos.z);
				if(newTileIndex != tileIndex)
				{
					if(pre == NULL)
						m_MonsterTiles[tileIndex] = m_MonsterTiles[tileIndex]->m_Next;
					else 
						pre->m_Next = monster->m_Next;

					monster->m_Flag = 0;
					Monster* p = monster;
					monster = pre;
					p->m_Next = m_MonsterTiles[newTileIndex];
					m_MonsterTiles[newTileIndex] = p;
				}
				else 
					monster->m_Flag = 0;
			}

			pre = monster;
			if(monster)
				monster = monster->m_Next;
			else 
				monster = m_MonsterTiles[tileIndex];
		}
	}
}

unsigned int MazeBaseScene::_GetRandomMonsterType()
{
	float threshold = 0.0f;
	float r = Ogre::Math::RangeRandom(0.0f, 1.0f);
	for (int i = 0; i < m_iNumMonsterType; i++)
	{
		threshold += m_MonsterTypeAndProbs[i].probability;
		if(r <= threshold)
			return m_MonsterTypeAndProbs[i].id;
	}
	return m_MonsterTypeAndProbs[m_iNumMonsterType - 1].id;
}

int MazeBaseScene::_GenerateMonsterGroupOnBattle( unsigned int monsterEncounter, unsigned int monsterIds[] )
{
	int num;
	float r = Ogre::Math::RangeRandom(0.0f, 1.0f);
	if(r < m_fRivalNumProbabilities[0])
		num = 1;
	else if(r < m_fRivalNumProbabilities[0] + m_fRivalNumProbabilities[1])
		num = 2;
	else 
		num = 3;

	monsterIds[0] = monsterEncounter;

	if(num == 1)
	{
		monsterIds[1] = 0;
		monsterIds[2] = 0;
	}
	else if(num == 2)
	{
		monsterIds[1] = _GetRandomMonsterType();
		monsterIds[2] = 0;
	}
	else if(num == 3)
	{
		monsterIds[1] = _GetRandomMonsterType();
		monsterIds[2] = _GetRandomMonsterType();
	}
	return num;
}
