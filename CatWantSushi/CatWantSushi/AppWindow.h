#pragma once
#include "Window.h"
class AppWindow :
	public Window
{
public:
	AppWindow();
	virtual ~AppWindow();

	void on_create() override;
	void on_update() override;
	void on_destroy() override;
};

