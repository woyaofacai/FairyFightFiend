#include "TestScene.h"
#include "GameApplication.h"

#define START_RUNNING_FRAME 1423
#define END_RUNNING_FRAME   1448
#define TOTAL_FRAME_NUM 1695
#define HEROINE_START_POS_X 962
#define HREOINE_START_POS_Z 937

static bool printQuery = false;

void TestScene::CreateScene()
{
	m_bOver = false;

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* ent = mSceneMgr->createEntity("MyPlane", "plane");

	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	//ent->setMaterialName("Examples/BeachStones");
	
	Ogre::Light* light1 = mSceneMgr->createLight("Light1");
	light1->setType(Ogre::Light::LT_DIRECTIONAL);
	light1->setDirection(1.0f, -1.0f, 0);
	light1->setDiffuseColour(1.0f, 1.0f, 1.0f);
	
	Ogre::Entity* heroine = mSceneMgr->createEntity("Heroine", "Zinade.mesh");

	m_HeroAnimState = heroine->getAnimationState("Run");
	m_RunAnimState = heroine->getAnimationState("Run");
	m_RunAnimState->setEnabled(false);
	m_RunAnimState->setLoop(false);

	m_WalkAnimState = heroine->getAnimationState("Run");
	m_WalkAnimState->setEnabled(true);
	m_WalkAnimState->setLoop(false);

	std::cout << "Center : " << heroine->getBoundingBox().getCenter() << std::endl;
	std::cout << "Half Height" << heroine->getBoundingBox().getHalfSize().y << std::endl;

	Ogre::SceneNode* heroNode = mSceneMgr->createSceneNode("HeroNode");
	heroNode->attachObject(heroine);
//	heroNode->translate(0, 0, 0);
	heroNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Math::PI));
//	heroNode->scale(0.05f, 0.05f, 0.05f);
	heroNode->scale(0.5f, 0.5f, 0.5f);


	m_PivotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PivotNode");
	m_PivotNode->setPosition(HEROINE_START_POS_X, 0, HREOINE_START_POS_Z);
	m_PivotNode->addChild(heroNode);


	m_HeroAnimState->setEnabled(false);
	m_HeroAnimState->setLoop(false);
	/*
	float animLength = heroine->getSkeleton()->getAnimation("All")->getLength();
	float animTimePerFrame = animLength / (TOTAL_FRAME_NUM - 1);
	
	m_fTimeStartRunning = animTimePerFrame * START_RUNNING_FRAME;
	m_fTimeEndRunning = animTimePerFrame * END_RUNNING_FRAME;
	m_HeroAnimState->setTimePosition(m_fTimeStartRunning);
	*/

	m_CamNode = m_PivotNode->createChildSceneNode("CamNode");
	//m_CamNode->translate(0, 100, 0);
	m_CamNode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(-Ogre::Math::PI / 12));
	m_CamNode->translate(0, 0, 30.0f, Ogre::SceneNode::TS_LOCAL);
	m_CamNode->attachObject(mCamera);

	std::string terrain_cfg("hill.cfg");
	mSceneMgr->setWorldGeometry( terrain_cfg );
	m_TerrainInfo.Load(terrain_cfg);

	mSceneMgr->setSkyBox(true, "SkyBox/Dusk", 500);
//	mViewport->setBackgroundColour(Ogre::ColourValue(0.78f, 0.92f, 0.95f, 1.0f));

	m_pRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	Ogre::ColourValue fadeColour(0.93f, 0.86f, 0.76f);
	mSceneMgr->setFog( Ogre::FOG_LINEAR, fadeColour, 0.001f, 100, 1000);
	
	Ogre::BillboardSet* bbset = mSceneMgr->createBillboardSet("BBSet", 4 * 100);
	bbset->setMaterialName("Hill/Tree");
	bbset->setSortingEnabled(true);
	bbset->setCullIndividually(true);
	// create a texture coordinate array to address the texture in Figure 10-4
	Ogre::FloatRect texCoordArray[] = {
		Ogre::FloatRect(0.0, 0.0, 0.5, 0.5),    // address the "A"
		Ogre::FloatRect(0.5, 0.0, 1.0, 0.5),    // address the "B"
		Ogre::FloatRect(0.0, 0.5, 0.5, 1.0),    // address the "C"
		Ogre::FloatRect(0.5, 0.5, 1.0, 1.0)     // address the "D"
	};
	bbset->setTextureCoords(texCoordArray, 4);
	for (int i = 0; i < 100; i++)
	{
		Ogre::Vector3 pos = _GetRandomPos();
		Ogre::Billboard* bb = bbset->createBillboard(pos.x, pos.y + 10.0f, pos.z);
		bb->setDimensions(20, 20);
		bb->setTexcoordIndex(1);
	}

	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(bbset);
	heroine->setRenderQueueGroup(10);

	_GenerateMonsterPositions("../media/positions/hill-monsters.txt", 100);

	_CreateMonsters();

//	Ogre::FontPtr font = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName("StarWars"));
	//Ogre::Font* font;
	//Ogre::FontPtr font = Ogre::FontManager::getSingleton().getByName("StarWars");
	//Ogre::Overlay* overlay = Ogre::OverlayManager::createOverlayElement()
	//mOverlay = Ogre::OverlayManager::getSingleton().getByName("TestOverlay");
	//mOverlay->show();

	//Ogre::OverlayManager *pOverlayManager = Ogre::OverlayManager::getSingletonPtr();
	//Ogre::OverlayElement* pText = static_cast<Ogre::OverlayElement*>(pOverlayManager->getOverlayElement("TextAreaOverlayElement1"));
//	pText->setCaption("hello");
	//std::cout << pText->getCaption() << std::endl;

	//CEGUI::System::
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	m_fpsText = wmgr.createWindow("GlossySerpent/StaticText", "MyButton");
	m_fpsText->setText("Quit");
	m_fpsText->setFont("MSYH-15");
	m_fpsText->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	sheet->addChildWindow(m_fpsText);

	CEGUI::System::getSingleton().setGUISheet(sheet);


	std::cout << "isHardwareAnimationEnabled : " << heroine->isHardwareAnimationEnabled() << std::endl;
	std::cout << heroine->getSubEntity(0)->getMaterialName();
	std::cout << heroine->getNumSubEntities();
	Ogre::Pass* pass = heroine->getSubEntity(0)->getMaterial()->getBestTechnique()->getPass(0);
	std::cout << "hasVertexProgram" << pass->hasVertexProgram() << std::endl;

	std::cout << "getNumBones : " << heroine->getSkeleton()->getNumBones() << std::endl;

	//m_TerrainInfo.Load(Ogre::String("hill.cfg"));
	//std::cout << m_TerrainInfo.m_fPageWorldWidth << std::endl;

	//m_CamNode->translate(0, 15, 30, Ogre::SceneNode::TS_WORLD);

//	Ogre::Entity* zombie = mSceneMgr->createEntity("Zombie.mesh");
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(zombie);


	Ogre::Entity* spider = mSceneMgr->createEntity("spider.mesh");

	Ogre::AnimationStateSet* animSet = spider->getAllAnimationStates();
	Ogre::AnimationStateIterator it = animSet->getAnimationStateIterator();
	while(it.hasMoreElements())
	{
		std::cout << "Spider: " << it.getNext()->getAnimationName() << std::endl;
	}

	//Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void TestScene::CreateCamera()
{
	mCamera = mSceneMgr->createCamera(mSceneName + "_MainCamera");
//	mCamera->setPosition(0, 0, 10);
	mCamera->setPosition(0, 0, 0);
	mCamera->lookAt(0, 0, -1);
	mCamera->setNearClipDistance(1.0f);
	mCamera->setFarClipDistance(1000.0f);

	//Ogre::Frustum* f = mCamera->getCullingFrustum();
}

bool TestScene::keyPressed(const OIS::KeyEvent &arg)
{
	if(arg.key == OIS::KC_ESCAPE)
	{
		GameApplication::GetSingletonPtr()->ReplaceScene(Ogre::String("GUITest"), true, true);
	}
	return true;
}

bool TestScene::frameStarted(const Ogre::FrameEvent& evt)
{
	if(mKeyboard->isKeyDown(OIS::KC_SPACE))
		printQuery = true;
	else 
		printQuery = false;


	if(m_bOver)
		return false;

	bool bWalked = false;
	bool bRun = false;

	Ogre::Vector3 trans(0, 0, 0);
	float fRot = 0.0f;

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
		
		m_PivotNode->translate(trans * evt.timeSinceLastFrame * HEROINE_WALK_SPEED, Ogre::Node::TS_LOCAL);
		
		m_WalkAnimState->addTime(evt.timeSinceLastFrame * 0.8f);
		if(m_WalkAnimState->hasEnded())
		{
			m_WalkAnimState->setTimePosition(0.0f);
		}
	}
	else if(bRun) {
		m_WalkAnimState->setEnabled(false);
		m_RunAnimState->setEnabled(true);
		m_PivotNode->translate(trans * evt.timeSinceLastFrame * HEROINE_RUN_SPEED, Ogre::Node::TS_LOCAL);

		m_RunAnimState->addTime(evt.timeSinceLastFrame);
		if(m_RunAnimState->hasEnded())
		{
			m_RunAnimState->setTimePosition(0.0f);
		}
	}

	
	if(bWalked || bRun)
	{
		Ogre::Ray ray;
		ray.setDirection(-Ogre::Vector3::UNIT_Y);
		ray.setOrigin(Ogre::Vector3(m_PivotNode->getPosition().x, m_PivotNode->getPosition().y + 10, m_PivotNode->getPosition().z));

		m_pRaySceneQuery->setRay(ray);
		//m_pRaySceneQuery->execute();

		Ogre::RaySceneQueryResult& result = m_pRaySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator resultIter = result.begin();

		
		if(resultIter != result.end() && resultIter->worldFragment)
		{
			if(resultIter->worldFragment->singleIntersection.y > 5.0f)
			{
				if(bWalked)
				{
					m_PivotNode->translate(-trans * evt.timeSinceLastFrame * HEROINE_WALK_SPEED, Ogre::Node::TS_LOCAL);
				}
				else {
					m_PivotNode->translate(-trans * evt.timeSinceLastFrame * HEROINE_RUN_SPEED, Ogre::Node::TS_LOCAL);
				}
			}
			else {
				m_PivotNode->setPosition(m_PivotNode->getPosition().x, resultIter->worldFragment->singleIntersection.y, m_PivotNode->getPosition().z);
			}
		}
	}

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
	
	m_fMonsterTileWidth = m_TerrainInfo.m_fPageWorldWidth / MONSTER_TILE_COL_NUM;

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
		if(indexes[i].col < 0 || indexes[i].col >= MONSTER_TILE_COL_NUM
			|| indexes[i].row < 0 || indexes[i].row >= MONSTER_TILE_COL_NUM)
			continue;

		tileIndex = indexes[i].row * MONSTER_TILE_COL_NUM + indexes[i].col;

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
				delete monster;
				GameApplication::GetSingletonPtr()->ReplaceScene(Ogre::String("Combat"), false, true);
				return true;
			}
			else if(dist < 2500.0f) 
			{
				//mCamera->getCullingFrustum()->
				Ogre::Frustum* frumstum = mCamera->getCullingFrustum();

				if(mCamera->isVisible(monster->GetSceneNode()->_getWorldAABB()))
				{
					monster->MoveTo(evt.timeSinceLastFrame, pivotPos);
					monster->m_Flag = 1;
				}
			}
			else if(dist < 10000.0f)
			{
				if(mCamera->isVisible(monster->GetSceneNode()->_getWorldAABB()))
				{
					monster->Idle(evt.timeSinceLastFrame);
				}
			}
			pre = monster;
			monster = monster->m_Next;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if(indexes[i].col < 0 || indexes[i].col >= MONSTER_TILE_COL_NUM
			|| indexes[i].row < 0 || indexes[i].row >= MONSTER_TILE_COL_NUM)
			continue;

		tileIndex = row * MONSTER_TILE_COL_NUM + col;

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


	/*
	Ogre::Ray ray;
	const Ogre::Vector3& camPos = m_CamNode->_getDerivedPosition();
	const Ogre::Vector3& pivotPos = m_PivotNode->_getDerivedPosition();
	ray.setOrigin(camPos);
	ray.setDirection(Ogre::Vector3(pivotPos.x - camPos.x, pivotPos.y + 5 - camPos.y, pivotPos.z - camPos.z));
	//ray.setDirection(Ogre::Vector3::UNIT_X);
	m_pRaySceneQuery->setRay(ray);
	//m_pRaySceneQuery->execute();
	m_pRaySceneQuery->clearResults();
	Ogre::RaySceneQueryResult& result = m_pRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator resultIter = result.begin();


	Ogre::Vector3 pos;
	bool flag = false;

	for (; resultIter != result.end(); resultIter++)
	{
		if(resultIter->worldFragment)
		{
			//std::cout << resultIter->worldFragment->singleIntersection.x << std::endl;
			Ogre::Vector3 pos = resultIter->worldFragment->singleIntersection;
			flag = true;
			break;
		}
	}

	if(flag)
	{
		pos.y = _GetTerrainHeight(pos.x, pos.z);

		float dist = pos.distance(camPos);

		//std::cout << dist << std::endl;
		if(dist < 30.0f)
		{
			//std::cout << resultIter->worldFragment->singleIntersection.x <<
			//	" " << resultIter->worldFragment->singleIntersection.y << 
			//	" " << resultIter->worldFragment->singleIntersection.z << std::endl;

			//float y = _GetTerrainHeight(resultIter->worldFragment->singleIntersection.x, resultIter->worldFragment->singleIntersection.z);
			//std::cout << y << " " << resultIter->worldFragment->singleIntersection.y << std::endl;
			//flag = true;
			//pos = resultIter->worldFragment->singleIntersection;
			//break;

			//std::cout << dist << std::endl;
			m_CamNode->translate(0, 0, -dist, Ogre::Node::TS_LOCAL);
		}
	}
	
	
	//std::cout << "Y : " << _GetTerrainHeight(923.243f, 744.629f) << std::endl;

	/*
	if(resultIter != result.end() && resultIter->worldFragment)
	{
		const Ogre::Vector3& w = resultIter->worldFragment->singleIntersection;
		if(w.x - camPos.x > 0 && w.x - pivotPos.x < 0 || w.x - camPos.x < 0 && w.x - pivotPos.x > 0)
		{
			float t = w.x - camPos.x / pivotPos.x - camPos.x;
			m_CamNode->translate(0, 0, -t * 30.0f, Ogre::Node::TS_LOCAL);
		}
	}
	*/

	

	/*
	std::list<Monster*>::iterator it = m_Monsters.begin();
	for (; it != m_Monsters.end(); it++)
	{
		Monster* monster = *it;
		if(monster->IsNearEnough(m_PivotNode->getPosition()))
		{
			mSceneMgr->destroySceneNode(monster->GetSceneNode());
			m_Monsters.erase(it);
			GameApplication::GetSingletonPtr()->ReplaceScene(Ogre::String("Combat"), false, true);
			break;
		}
	}
	*/

	/*
	if(mKeyboard->isKeyDown(OIS::KC_SPACE))
	{
		std::cout << "x:" << m_PivotNode->getPosition().x << " y:" << m_PivotNode->getPosition().y << " z:" << m_PivotNode->getPosition().z << std::endl;
	}
	*/
	
	float fps = GameApplication::GetSingletonPtr()->GetWindow()->getLastFPS();
	char fpsStr[20];
	sprintf(fpsStr, "%f", fps);
	m_fpsText->setText(Ogre::String(fpsStr));

	return true;
}

float TestScene::_GetTerrainHeight(float x, float z)
{
	Ogre::Vector3 pos;
	pos.x = x;
	pos.z = z;
	pos.y = 200.0f;

	Ogre::Ray ray;
	ray.setDirection(-Ogre::Vector3::UNIT_Y);
	ray.setOrigin(pos);
	m_pRaySceneQuery->setRay(ray);
//	m_pRaySceneQuery->execute();

	m_pRaySceneQuery->clearResults();
	Ogre::RaySceneQueryResult& result = m_pRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin( );
	if(it != result.end() && it->worldFragment)
	{
		pos.y = it->worldFragment->singleIntersection.y;
	}
	return pos.y;
}

void TestScene::CreateSceneManager()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

void TestScene::_CreateMonsters()
{
	memset((void*)m_MonsterTiles, 0, MONSTER_TILE_NUM * sizeof(Monster*));

	Ogre::SceneNode* monstersRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	std::ifstream infile;
	infile.open("../media/positions/hill-monsters.txt");

	Ogre::Vector3 pos;
	Ogre::String monsterTypeName;
	Ogre::String monsterMeshName;
	pos.y = 0;
	while(!infile.eof())
	{
		infile >> monsterTypeName >> pos.x >> pos.z;
		MonsterInfo* pMonsterInfo = MonsterInfoManager::GetSingleton().FindMonsterInfo(monsterTypeName);
		Ogre::Entity* monsterEnt = mSceneMgr->createEntity(Ogre::String(pMonsterInfo->m_MeshName));

		monsterEnt->setRenderQueueGroup(10);
		Ogre::SceneNode* monsterNode = mSceneMgr->createSceneNode();
		monsterNode->attachObject(monsterEnt);

		monsterNode->scale(pMonsterInfo->m_fScale, pMonsterInfo->m_fScale, pMonsterInfo->m_fScale);
		monstersRootNode->addChild(monsterNode);
		monsterNode->setPosition(pos);
		Monster* monster = new Monster(monsterNode, pMonsterInfo);

		int tileIndex = _GetMonsterTile(pos.x, pos.z);
		monster->m_Next = m_MonsterTiles[tileIndex];
		m_MonsterTiles[tileIndex] = monster;
	}
	infile.close();
}

int TestScene::_GetMonsterTile(float x, float z, int* pCol /* = NULL */, int* pRow /* = NULL */)
{
	int col = Ogre::Math::Floor(x / m_TerrainInfo.m_fPageWorldWidth * MONSTER_TILE_COL_NUM);
	int row = Ogre::Math::Floor(z / m_TerrainInfo.m_fPageWorldWidth * MONSTER_TILE_COL_NUM);

	//std::cout << "col: " << col << " row: " << row <<std::endl;

	if(pCol != NULL) *pCol = col;
	if(pRow != NULL) *pRow = row;
	return row * MONSTER_TILE_COL_NUM + col;
}

#ifdef GENERATE_HILLS_CONFIG_FILE

Ogre::Vector3 TestScene::_GetRandomPos()
{
	Ogre::Vector3 pos;
	pos.x = Ogre::Math::RangeRandom(0, 2000.0f);
	pos.z = Ogre::Math::RangeRandom(0, 2000.0f);
	pos.y = 200.0f;

	Ogre::Ray ray;
	ray.setDirection(-Ogre::Vector3::UNIT_Y);
	ray.setOrigin(pos);
	m_pRaySceneQuery->setRay(ray);
	//m_pRaySceneQuery->execute();

	Ogre::RaySceneQueryResult& result = m_pRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin( );
	if(it != result.end() && it->worldFragment)
	{
		pos.y = it->worldFragment->singleIntersection.y;
	}
	return pos;
}

void TestScene::_GenerateMonsterPositions(LPCSTR filename, int num)
{
	std::ofstream outfile, xmlfile;
	outfile.open(filename);
	xmlfile.open(Ogre::String(filename) + ".xml");

	Ogre::Vector3 pos;

	for (int i = 0; i < num; i++)
	{
		do {
			pos = _GetRandomPos();
		} while(pos.y != 0);

		Ogre::String monsterMeshName;
		if(rand() % 2 == 0) 
			monsterMeshName = "Zombie";
		else 
			monsterMeshName = "Spider";

		outfile << monsterMeshName << " " << pos.x << " " << pos.z << std::endl;
		
		char s[100];
		sprintf(s, "<monster name=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"></monster>", monsterMeshName.c_str(), pos.x, pos.y, pos.z);
		xmlfile << s << std::endl;
	}
	outfile.close();
	xmlfile.close();
}



#endif