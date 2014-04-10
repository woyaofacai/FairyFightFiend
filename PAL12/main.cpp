//#include <OGRE/ExampleApplication.h>

//#include "GameApplication.h"
#include "GameApplicationImpl.h"


#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "OIS_d.lib")
#pragma comment(lib, "CEGUIBase_d.lib")
#pragma comment(lib, "CEGUIOgreRenderer_d.lib")
#pragma comment(lib, "irrKlang.lib")

int main()
{
	srand(time(NULL));
	GameApplicationImpl app;
	GameApplication::SetSingleton(&app);
	app.StartUp();
	return 0;
}