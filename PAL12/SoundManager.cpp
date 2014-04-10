#include "SoundManager.h"
#include <string.h>

SoundManager::SoundManager(void)
{
	m_engine = createIrrKlangDevice();
}


SoundManager::~SoundManager(void)
{
}

SoundManager& SoundManager::GetSingleton()
{
	static SoundManager sm;
	return sm;
}

ISound* SoundManager::GetSound( const char* filename, bool loop)
{
	std::map<Ogre::String, irrklang::ISound*>::iterator it = m_Sounds.find(Ogre::String(filename));
	if(it != m_Sounds.end())
	{
		return it->second;
	}

	char s[100];
	strcpy(s, SOUND_FILE_PATH);
	strcat(s, filename);

	ISoundSource* source = m_engine->getSoundSource(s);
	if(!source)
		source = m_engine->addSoundSourceFromFile(s);

	ISound* sound = m_engine->play2D(source, loop, true, true, true);
	m_Sounds.insert(std::pair<Ogre::String, irrklang::ISound*>(Ogre::String(filename), sound));
	return sound;
}

void SoundManager::StopAllSounds()
{
	m_engine->stopAllSounds();
}

void SoundManager::PauseAllSounds(bool bPaused)
{
	m_engine->setAllSoundsPaused(true);
}
