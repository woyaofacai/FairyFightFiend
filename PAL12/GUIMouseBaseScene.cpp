#include "GUIMouseBaseScene.h"

bool GUIMouseBaseScene::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System& sys = CEGUI::System::getSingleton();
	sys.injectMouseButtonUp(_ConvertButton(id));
	return true;
}

bool GUIMouseBaseScene::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System& sys = CEGUI::System::getSingleton();
	sys.injectMouseButtonDown(_ConvertButton(id));
	return true;
}

bool GUIMouseBaseScene::mouseMoved(const OIS::MouseEvent &arg)
{
	CEGUI::System& sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);

	if(arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}

CEGUI::MouseButton GUIMouseBaseScene::_ConvertButton(OIS::MouseButtonID buttonID)
{
	switch(buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
	case OIS::MB_Right:
		return CEGUI::RightButton;
	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
	}
	return CEGUI::LeftButton;
}