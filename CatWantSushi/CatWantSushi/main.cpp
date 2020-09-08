#include "Window.h"
#include "BitmapManager.h"
#include "GameOptionManager.h"


int main()
{
	BitmapManager::GetInstance();
	GameOptionManager::GetInstance();
	Window app;
	if (app.init())
	{
		while(app.is_run())
		{
			app.broadcast();
		}
	}
	return 0;
}