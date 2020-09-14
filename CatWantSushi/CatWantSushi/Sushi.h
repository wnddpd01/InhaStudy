#pragma once
#include "Object.h"
#include "BitmapManager.h"
#include "InGameScene.h"
#include "resource.h"

class Sushi :
	public Object
{
	const static UCHAR sushi_length = 5;
public:
	UCHAR sushi_score;
	
	Sushi(UCHAR sushi_id, LONG  pos_x, LONG pos_y) : Object(sushi_id, NULL, pos_x, pos_y, sushi_length, sushi_length)
	{
		sushi_score = sushi_id - OBJECT_SUSHI_EGG + 1;
		switch (sushi_id)
		{
		case OBJECT_SUSHI_EGG:
			object_bitmap_id_ = BITMAP_SUSHI_EGG;
			break;
		case OBJECT_SUSHI_SHRIMP:
			object_bitmap_id_ = BITMAP_SUSHI_SHRIMP;
			break;
		case OBJECT_SUSHI_TUNA:
			object_bitmap_id_ = BITMAP_SUSHI_TUNA;
			break;
		}
	}
};

