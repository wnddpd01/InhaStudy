#include "SushiGenerator.h"

#include <iostream>
#include <random>
#include "Object.h"
#include "InGameScene.h"
#include "GameOptionManager.h"
#include "Sushi.h"

extern GameOptionManager* game_option_manager;

void SushiGenerator::update()
{
	
}

Object* SushiGenerator::sushi_generation()
{
	const static UCHAR sushi_length = 5;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> sushi_type_dist(0, 2);
	static std::uniform_int_distribution<int> horizontal_dist(0, INT(game_option_manager->HorizontalGridCount) - sushi_length);
	static std::uniform_int_distribution<int> vertical_dist(3, INT(game_option_manager->VerticalGridCount) - sushi_length - 3);
	UCHAR sushi_type = sushi_type_dist(rd);
	Sushi* sushi = new Sushi(OBJECT_SUSHI_EGG + sushi_type, horizontal_dist(gen), vertical_dist(rd));

	sushi->render = &Object::move_updown_render;
	return sushi;
}
