#ifndef _GUIMOUSEBASESCENE_H
#define _GUIMOUSEBASESCENE_H

#include "GameScene.h"
#include <CEGUI.h>

class GUIMouseBaseScene : public BaseScene
{
public:
	virtual bool mouseMoved (const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
	CEGUI::MouseButton _ConvertButton(OIS::MouseButtonID buttonID);
};

#endif

