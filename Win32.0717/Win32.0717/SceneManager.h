#pragma once
#include "StartScene.h"
#include "GameScene.h"
class CSceneManager
{
public:
	CScene * m_scene;
	CStartScene * m_startScene;
	CGameScene * m_gameScene;
	SceneState m_curState;
	CSceneManager()
	{
		Init();
	}

	~CSceneManager()
	{
	}

	void Init()
	{
		m_curState = SceneState::start;
		m_scene = m_startScene = new CStartScene;
		m_scene->Init();
	}

	void Update(UINT message, WPARAM wParam, LPARAM lParam)
	{
		m_scene->Update(message, wParam, lParam);
	}

	void Render(HDC hdc)
	{
		m_scene->Render(hdc);
	}

	void SceneChange(SceneState nextScene)
	{
		switch (nextScene)
		{
		case SceneState::start:
			break;
		case SceneState::ingame:
			if (m_gameScene == nullptr)
				m_gameScene = new CGameScene;
			m_gameScene->Init();
			m_scene = m_gameScene;
			break;
		}
	}
};

