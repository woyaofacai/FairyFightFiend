#ifndef _GUITESTSCENE_H
#define _GUITESTSCENE_H

#include "GUIMouseBaseScene.h"
#include "GameApplication.h"

class GUITestScene : public GUIMouseBaseScene
{
public:
	GUITestScene(void);
	~GUITestScene(void);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool keyPressed (const OIS::KeyEvent &arg);
	virtual void CreateScene();
	virtual void SetActive();
	virtual void SetUnactive();
	bool quit(const CEGUI::EventArgs& arg);
	bool newStory(const CEGUI::EventArgs& arg);

private:
	CEGUI::Window* m_guiWindow;
	CEGUI::Window* m_NewBtn;
	CEGUI::Window* m_QuitBtn;
	bool m_bQuit;
};

#endif

