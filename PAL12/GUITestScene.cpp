#include "GUITestScene.h"

GUITestScene::GUITestScene(void)
{

}

GUITestScene::~GUITestScene(void)
{

}

void GUITestScene::CreateScene()
{
	m_bQuit = false;

	m_guiWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("PAL12.layout");
	m_guiWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	CEGUI::System::getSingleton().setGUISheet(m_guiWindow);

	//CEGUI::Window* ceWindow = CEGUI::WindowManager::getSingleton().getWindow("root");
	m_NewBtn = CEGUI::WindowManager::getSingleton().getWindow("NewButton");
	m_QuitBtn = CEGUI::WindowManager::getSingleton().getWindow("QuitButton");

	m_QuitBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUITestScene::quit, this));
	m_NewBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUITestScene::newStory, this));

	//CEGUI::MouseCursor::getSingleton().show();
}

bool GUITestScene::quit(const CEGUI::EventArgs& arg)
{
	m_bQuit = true;
	return true;
}

bool GUITestScene::frameStarted(const Ogre::FrameEvent& evt)
{
	if(m_bQuit)
		return false;

	CEGUI::System& sys = CEGUI::System::getSingleton();
	sys.injectTimePulse(evt.timeSinceLastFrame);
	return true;
}

bool GUITestScene::keyPressed(const OIS::KeyEvent &arg)
{
	if(arg.key == OIS::KC_ESCAPE)
		m_bQuit = true;

	return true;
}

bool GUITestScene::newStory(const CEGUI::EventArgs& arg)
{
	GameApplication::GetSingletonPtr()->ReplaceScene(Ogre::String("Valley"), false, true);
	return false;
}

void GUITestScene::SetActive()
{
	
	//CEGUI::System::getSingleton().updateWindowContainingMouse();
	CEGUI::MouseCursor::getSingleton().show();
	CEGUI::System::getSingleton().setGUISheet(m_guiWindow);
	CEGUI::System::getSingleton().signalRedraw();
	
}

void GUITestScene::SetUnactive()
{
//	m_guiWindow->destroy();
	CEGUI::System::getSingleton().setGUISheet(NULL);
	CEGUI::MouseCursor::getSingleton().hide();
//	m_bSceneCreated = false;
}

