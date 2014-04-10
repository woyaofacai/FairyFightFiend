#include "CombatScene.h"
#include "GameApplication.h"
#include "MonsterInfoManager.h"
#include <OGRE\OgreFontManager.h>


CombatScene::CombatScene(void)
{
	
}


CombatScene::~CombatScene(void)
{

}

void CombatScene::CreateScene()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 100, 100, 200, 200, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* ent = mSceneMgr->createEntity("MyPlane", "plane");

	Ogre::Light* light1 = mSceneMgr->createLight("Light1");
	light1->setType(Ogre::Light::LT_DIRECTIONAL);
	light1->setDirection(1.0f, -1.0f, 0);
	light1->setDiffuseColour(1.0f, 1.0f, 1.0f);

	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	ent->setMaterialName("Ground/GreenSkin");

	m_RivalsRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RivalsRootNode");
	
	Ogre::Entity* hero = mSceneMgr->createEntity("Heroine", "zidane.mesh");
	m_HeroNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeroNode");
	m_HeroNode->attachObject(hero);
	m_HeroNode->setPosition(Rival::HERO_POSITION);
	m_HeroNode->scale(0.4f, 0.4f, 0.4f);
	m_HeroNode->yaw(Ogre::Radian(-Ogre::Math::HALF_PI));
	m_Hero = new HeroRival(m_HeroNode, hero, mSceneName);

	Ogre::Entity* pyramid = mSceneMgr->createEntity("pyramid.mesh");
	pyramid->setMaterialName("PyramidMaterial");
	m_PyramidNode = mSceneMgr->createSceneNode();
	//m_PyramidNode->translate(0, 5.0f, 0);
	m_PyramidNode->scale(0.04f, 0.04f, 0.04f);
	m_PyramidNode->attachObject(pyramid);
	mSceneMgr->getRootSceneNode()->addChild(m_PyramidNode);


	m_BackMusic = SoundManager::GetSingleton().GetSound("battle.mp3", true);

	m_guiWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("combat.layout");
	m_guiWindow->setArea(CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.7f, 0)), 
		CEGUI::UVector2(CEGUI::UDim(0.27f, 0), CEGUI::UDim(0.26f, 0)));
	CEGUI::System::getSingleton().setGUISheet(m_guiWindow);
	
	
	m_CameraControlPoints[0] = Ogre::Vector3(6.0f, 3.0f, -5.0f);
	m_CameraControlPoints[1] = Ogre::Vector3(8.0f, 3.0f, 0.0f);
	m_CameraControlPoints[2] = Ogre::Vector3(9.0f, 3.0f, 2.0f);


	Ogre::FontPtr font = Ogre::FontManager::getSingleton().getByName("StarWars");
	font->load();
	
	Ogre::Overlay* overlay = (Ogre::Overlay*) Ogre::OverlayManager::getSingleton().getByName("TestOverlay");
	Ogre::OverlayElement* element = (Ogre::OverlayElement*)Ogre::OverlayManager::getSingleton().getOverlayElement("TextAreaOverlayElement1");
	element->setCaption("HELLO");
	element->show();
	element->setColour(Ogre::ColourValue(1.0f, 0.0f, 0.0f, 0.3f));

	


	Ogre::Vector3 p1 = Rival::RIVAL_POSITIONS[0];
	Ogre::Vector4 p2(p1.x, p1.y, p1.z, 1);
	p2 = p2 * mCamera->getViewMatrix() * mCamera->getProjectionMatrix();
	p2 /= p2.w;

	std::cout << "P2:(" << p2.x << ", " << p2.y << ", " << p2.z << ", " << p2.w << ")" << std::endl;


	 overlay->show();
}

void CombatScene::CreateCamera()
{
	mCamera = mSceneMgr->createCamera("CombatCamera");
	mCamera->setPosition(30, 10, 0);
	mCamera->lookAt(0, 0, 0);
	mCamera->setNearClipDistance(1.0f);
	mCamera->setFarClipDistance(1000.0f);
}

bool CombatScene::frameStarted( const Ogre::FrameEvent& evt )
{
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		GameApplication::GetSingletonPtr()->ReplaceScene(Ogre::String("Valley"), true, true);
		return true;
	}

	for (int i = 0; i < 3; i++)
	{
		if(m_Rivals[i] != NULL)
		{
			m_Rivals[i]->Update(evt.timeSinceLastFrame);
		}
	}
	m_Hero->Update(evt.timeSinceLastFrame);
	
	if(m_CurState == RIVAL_ATTACK)
	{
		if(m_Rivals[m_iRivalFocused]->GetCurrentState().m_MainState != Rival::ATTACK)
		{
			m_iRivalFocused++;
			while(m_Rivals[m_iRivalFocused] == NULL)
			{
				m_iRivalFocused++;
			}

			if(m_iRivalFocused < 3)
				m_Rivals[m_iRivalFocused]->BeginAttack();
			else 
				_ConvertState(SELECT_TACTICS);
		}
	}
	else if(m_CurState == HERO_ATTACK)
	{
		if(m_Hero->GetCurrentState().m_MainState == Rival::IDLE)
		{
			_ConvertState(RIVAL_ATTACK);
		}
	}
	else if(m_CurState == SELECT_RIVAL)
	{
		m_PyramidNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(evt.timeSinceLastFrame));
	}
	
	_UpdateCameraPosition(evt);

	CEGUI::System& sys = CEGUI::System::getSingleton();
	sys.injectTimePulse(evt.timeSinceLastFrame);

	return true;
}

void CombatScene::SetActive()
{
	m_Rivals[0] = NULL; 
	m_Rivals[1] = NULL;
	m_Rivals[2] = NULL;

	if(m_iMonsterNum == 1)
	{
		_AddMonsterOnPosition(m_MonsterTypes[0], 1);
	}
	else if(m_iMonsterNum == 2)
	{
		int pos1, pos2, empty;
		_GetRandomRivalPosition(&pos1, &pos2, &empty);
		_AddMonsterOnPosition(m_MonsterTypes[0], pos1);
		_AddMonsterOnPosition(m_MonsterTypes[1], pos2);
	}
	else if(m_iMonsterNum == 3)
	{
		int pos1, pos2, pos3;
		_GetRandomRivalPosition(&pos1, &pos2, &pos3);
		_AddMonsterOnPosition(m_MonsterTypes[0], pos1);
		_AddMonsterOnPosition(m_MonsterTypes[1], pos2);
		_AddMonsterOnPosition(m_MonsterTypes[2], pos3);
	}

	m_BackMusic->setPlayPosition(0);
	m_BackMusic->setIsPaused(false);

	_ConvertState(SELECT_TACTICS);
	m_HeroNode->setPosition(Rival::HERO_POSITION);
	//m_HeroNode->setDirection(Ogre::Vector3::NE);
	//m_Hero->SetState(Rival::IDLE, Rival::IDLE);
	m_Hero->SetIdle();


	CEGUI::MouseCursor::getSingleton().show();
	CEGUI::System::getSingleton().setGUISheet(m_guiWindow);
	CEGUI::System::getSingleton().signalRedraw();

	m_AttackBtn = CEGUI::WindowManager::getSingleton().getWindow("AttackButton");
	m_AttackBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CombatScene::OnAttackButtonClicked, this));

}

void CombatScene::SetUnactive()
{
	delete m_Rivals[0];
	delete m_Rivals[1];
	delete m_Rivals[2];

	m_Rivals[0] = NULL; 
	m_Rivals[1] = NULL;
	m_Rivals[2] = NULL;

	m_RivalsRootNode->removeAndDestroyAllChildren();

	m_BackMusic->setIsPaused(true);

	CEGUI::MouseCursor::getSingleton().hide();
	CEGUI::System::getSingleton().setGUISheet(NULL);
	CEGUI::System::getSingleton().signalRedraw();
}

void CombatScene::_GetRandomRivalPosition( int* first, int* second, int* third )
{
	int r = rand() % 6;
	switch(r)
	{
	case 0:
		*first = 0; *second = 1; *third = 2;
		break;
	case 1:
		*first = 1; *second = 0; *third = 2;
		break;
	case 2:
		*first = 0; *second = 2; *third = 1;
		break;
	case 3:
		*first = 2; *second = 0; *third = 1;
		break;
	case 4:
		*first = 1; *second = 2; *third = 0;
		break;
	case 5:
		*first = 2; *second = 1; *third = 0;
		break;
	}
}

void CombatScene::_AddMonsterOnPosition( unsigned int id, int pos )
{
	const MonsterInfo* pMonsterInfo = MonsterInfoManager::GetSingleton().FindMonsterInfo(id);
	Ogre::Entity* ent = mSceneMgr->createEntity(Ogre::String(pMonsterInfo->m_MeshName));
	Ogre::SceneNode* rivalNode = m_RivalsRootNode->createChildSceneNode();
	rivalNode->attachObject(ent);
	rivalNode->scale(pMonsterInfo->m_fScale, pMonsterInfo->m_fScale, pMonsterInfo->m_fScale);
	rivalNode->setPosition(Rival::RIVAL_POSITIONS[pos]);
	rivalNode->yaw(Ogre::Radian(Ogre::Math::HALF_PI));

	Rival* rival = new MonsterRival(rivalNode, ent, mSceneName, pMonsterInfo, pos);
	rival->SetTargetRival(m_Hero);
	m_Rivals[pos] = rival;
}

bool CombatScene::mouseMoved( const OIS::MouseEvent &arg )
{
	if(m_CurState == SELECT_TACTICS || m_CurState == SELECT_RIVAL)
		GUIMouseBaseScene::mouseMoved(arg);

	if(m_CurState == SELECT_RIVAL)
	{
		m_iRivalFocused = _GetMonsterPosNumToAttack(arg);
		if(m_iRivalFocused != -1)
		{
			Ogre::Vector3 pyramidPos = Rival::RIVAL_POSITIONS[m_iRivalFocused];
			pyramidPos.y = m_Rivals[m_iRivalFocused]->GetHeight() + 3.0f;
			//std::cout << pyramidPos.y << std::endl;
			m_PyramidNode->setPosition(pyramidPos);
			//m_PyramidNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(m_CameraMoveTimer.getMicroseconds() * 0.001f * 0.01f));
			m_PyramidNode->setVisible(true);
		}
		else {
			m_PyramidNode->setVisible(false);
		}
	}

	return true;
}

bool CombatScene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(m_CurState == SELECT_TACTICS || m_CurState == SELECT_RIVAL)
		GUIMouseBaseScene::mousePressed(arg, id);

	if(m_CurState == SELECT_RIVAL)
	{
		//m_iRivalFocused = _GetMonsterPosNumToAttack(arg);
		if(m_iRivalFocused != -1)
		{
			_ConvertState(HERO_ATTACK);
		}
	}

	CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();

	return true;
}

bool CombatScene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(m_CurState == SELECT_TACTICS || m_CurState == SELECT_RIVAL)
		GUIMouseBaseScene::mouseReleased(arg, id);

	return true;
}

int CombatScene::_GetMonsterPosNumToAttack(const OIS::MouseEvent &arg)
{
	int posNum = -1;

	CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
	Ogre::Ray ray = mCamera->getCameraToViewportRay((float)mousePos.d_x / (float)arg.state.width, (float)mousePos.d_y / (float)arg.state.height);
	// calculate intersection of the ray and the plane : n = (1, 0, 0), d = 15
	float t = -(15.0f + ray.getOrigin().x) / ray.getDirection().x; 
	Ogre::Vector3 intersectPoint = ray.getOrigin() + ray.getDirection() * t;

	if(intersectPoint.y > -2.0f && intersectPoint.y < 7.0f)
	{
		if(intersectPoint.z <= 7.5f && intersectPoint.z >= -7.5f)
			posNum = 1;
		else if(intersectPoint.z >= -22.5f && intersectPoint.z <= -7.5f)
			posNum = 2;
		else if(intersectPoint.z <= 22.5f && intersectPoint.z >= 7.5f)
			posNum = 0;
	}

	if(posNum != -1 && m_Rivals[posNum] == NULL)
		posNum = -1;

	return posNum;
}

int CombatScene::_GetFirstValidMonsterPosNum()
{
	int pos = 0;
	while(m_Rivals[pos] == NULL)
		pos++;
	return pos;
}

void CombatScene::_ConvertState( CombatStateEnum state )
{
	if(state == SELECT_TACTICS)
	{
		CEGUI::MouseCursor::getSingleton().show();
		m_CurState = SELECT_TACTICS;
		m_guiWindow->show();
		m_CameraMoveTimer.reset();
		m_PyramidNode->setVisible(false);
	}
	else if(state == SELECT_RIVAL) {
		CEGUI::MouseCursor::getSingleton().show();
		m_CurState = SELECT_RIVAL;
		m_guiWindow->hide();
		mCamera->setPosition(30.0f, 10.0f, 0.0f);
		mCamera->lookAt(0.0f, 0.0f, 0.0f);
	}
	else if(state == HERO_ATTACK) {
		CEGUI::MouseCursor::getSingleton().hide();
		m_CurState = HERO_ATTACK;
		//m_Hero->SetTargetPos(Rival::RIVAL_POSITIONS[m_iRivalFocused]);
		m_Hero->SetTargetRival(m_Rivals[m_iRivalFocused]);
		m_Hero->BeginAttack();
		m_PyramidNode->setVisible(false);

	}
	else if(state == RIVAL_ATTACK)
	{
		CEGUI::MouseCursor::getSingleton().hide();
		m_CurState = RIVAL_ATTACK;
		m_iRivalFocused = _GetFirstValidMonsterPosNum();
		m_Rivals[m_iRivalFocused]->BeginAttack();
		m_PyramidNode->setVisible(false);
	}
	
}

bool CombatScene::OnAttackButtonClicked( const CEGUI::EventArgs& arg )
{
	_ConvertState(SELECT_RIVAL);
	return true;
}

void CombatScene::_UpdateCameraPosition(const Ogre::FrameEvent& evt)
{
	if(m_CurState == SELECT_TACTICS)
	{
		if(m_CameraMoveTimer.getMilliseconds() < 3000)
		{
			float t = (float)m_CameraMoveTimer.getMilliseconds() / 3000.0f;
			Ogre::Vector3 camPos = m_CameraControlPoints[0] * ((1 - t) * (1 - t)) 
				+ m_CameraControlPoints[1] * (2 * t * (1 - t))
				+ m_CameraControlPoints[2] * (t * t);

			mCamera->setPosition(camPos);
			mCamera->lookAt(15.0f, 3.0f, 0.0f);
		}
	}
}

void CombatScene::KillMonster()
{
	Ogre::SceneNode* rivalNode = m_Rivals[m_iRivalFocused]->GetSceneNode();
	m_RivalsRootNode->removeChild(rivalNode);
	delete m_Rivals[m_iRivalFocused];
	m_Rivals[m_iRivalFocused] = NULL;
}

void CombatScene::OnMessage( Ogre::uint32 id, Ogre::int32 data, void* lpinfo )
{
	switch(id)
	{
	case KILL_RIVAL_MSG:
		{
			Rival* rival = (Rival*)lpinfo;
			if(rival != m_Hero)
			{
				int rivalIndex = 0;
				while(m_Rivals[rivalIndex] != rival) rivalIndex++;
				Ogre::SceneNode* rivalNode = rival->GetSceneNode();
				m_RivalsRootNode->removeChild(rivalNode);
				delete rival;
				m_Rivals[rivalIndex] = NULL;
			}
		}
		break;
	}
}
