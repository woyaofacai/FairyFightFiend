#ifndef _SOUND_MANAGER_H
#define _SOUND_MANAGER_H

#include <irrKlang.h>
#include <ogre/Ogre.h>

using namespace irrklang;

const char* const SOUND_FILE_PATH = "../media/sound/";

class SoundManager
{
public:
	static SoundManager& GetSingleton();
	ISound* GetSound(const char* filename, bool loop = false);
	void StopAllSounds();
	void PauseAllSounds(bool bPaused = true);
	~SoundManager(void);
private:
	SoundManager(void);
	irrklang::ISoundEngine* m_engine;
	std::map<Ogre::String, irrklang::ISound*> m_Sounds;
};

#endif