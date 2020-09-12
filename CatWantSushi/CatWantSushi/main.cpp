#include <thread>

#include "Window.h"
#include "BitmapManager.h"
#include "GameOptionManager.h"
#include "SceneManager.h"

auto bitmap_manager = BitmapManager::GetInstance();
auto game_option_manager = GameOptionManager::GetInstance();
auto scene_manager = SceneManager::getInstance();


int main()
{
	Window app;
	
	if (app.init())
	{
		std::thread game_loop_thread(&Window::on_update, &app);
		while(app.is_run())
		{
			app.broadcast();
		}
		game_loop_thread.join();
	}
	return 0;
}