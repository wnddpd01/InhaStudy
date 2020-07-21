#include "stdafx.h"

extern Singleton * singleton;

void CSceneManager::SceneChange(SceneState nextScene)
{
	m_scene->Free();
	switch (nextScene)
	{
	case SceneState::start:
		m_scene = m_startScene;
		break;
	case SceneState::ingame:
		if (m_gameScene == nullptr)
			m_gameScene = new CGameScene;
		m_scene = m_gameScene;
		break;
	case SceneState::end:
		if (m_endScene == nullptr)
			m_endScene = new CEndScene;
		m_scene = m_endScene;
		break;
	case SceneState::quit :
		m_scene->Free();
		PostQuitMessage(0);
		return;
		break;
	}
	m_scene->Init();
}
