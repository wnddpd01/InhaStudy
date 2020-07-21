#pragma once
#include "StartScene.h"
#include "GameScene.h"
#include "EndScene.h"
class CSceneManager
{
public:
	CScene * m_scene;
	CStartScene * m_startScene;
	CGameScene * m_gameScene;
	CEndScene * m_endScene;
	SceneState m_curState;
	CSceneManager()
	{
		m_gameScene = nullptr;
		m_endScene = nullptr;
		m_scene = nullptr;
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

	void SceneChange(SceneState nextScene);

};

