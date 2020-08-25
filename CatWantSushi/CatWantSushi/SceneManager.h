#pragma once
#include <Windows.h>

class Scene;
class StartScene;

class SceneManager
{
	SceneManager();
	StartScene * start_scene_;
	Scene * cur_scene_;
public:
#pragma region GetterAndSetter
	Scene* cur_scene() const
	{
		
		return cur_scene_;
	}

	void set_cur_scene(Scene* cur_scene)
	{
		cur_scene_ = cur_scene;
	}

	__declspec(property(get = cur_scene, put = set_cur_scene)) Scene* CurScene;
#pragma endregion

	static SceneManager * getInstance()
	{
		static SceneManager ins;
		return &ins;
	}

	void render(LPPAINTSTRUCT ps);
	void scene_change(WPARAM next_scene);
	~SceneManager();
};
