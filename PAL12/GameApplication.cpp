#include "GameApplication.h"

GameApplication* GameApplication::m_pSingleton = NULL;

GameApplication* GameApplication::GetSingletonPtr()
{
	return m_pSingleton;
}

void GameApplication::SetSingleton( GameApplication* pSingleton )
{
	m_pSingleton = pSingleton;
}
