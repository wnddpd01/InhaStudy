#include "AppWindow.h"

int main()
{
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