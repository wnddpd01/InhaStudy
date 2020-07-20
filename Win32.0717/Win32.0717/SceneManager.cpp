#include "stdafx.h"

extern Singleton * singleton;

void CSceneManager::SceneChange(SceneState nextScene)
{
	switch (nextScene)
	{
	case SceneState::start:
		break;
	case SceneState::ingame:
		if (m_gameScene == nullptr)
			m_gameScene = new CGameScene;
		m_scene = m_gameScene;
		break;
	}
	m_scene->Init();
}
