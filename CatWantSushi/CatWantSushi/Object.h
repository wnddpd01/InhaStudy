#pragma once
#include "GameOptionManager.h"
#include "BitmapManager.h"
struct POINT_UCHAR
{
	UCHAR x;
	UCHAR y;
};

enum direction : BOOL
{
	dir_left = 0, dir_right
};
class Object
{

protected:
	UINT object_id_;
	UINT object_bitmap_id_;
	POINT object_pos_;
	POINT_UCHAR object_width_height_;
	RECT object_rect_;
	direction direction_;
	
	void set_object_rect()
	{
		GameOptionManager* game_option_manager = GameOptionManager::GetInstance();
		object_rect_.left = game_option_manager->GameCellSize * object_pos_.x;
		object_rect_.top = game_option_manager->GameCellSize * object_pos_.y;
		object_rect_.right = game_option_manager->GameCellSize * object_width_height_.x + object_rect_.left;
		object_rect_.bottom = game_option_manager->GameCellSize * object_width_height_.y + object_rect_.top;
	}

	void set_object_pos()
	{
		GameOptionManager* game_option_manager = GameOptionManager::GetInstance();
		if(direction_ == dir_left)
			object_pos_.x = LONG(object_rect_.left / game_option_manager->GameCellSize);
		else
			object_pos_.x = LONG(object_rect_.right / game_option_manager->GameCellSize - object_width_height_.x + 1);
		object_pos_.y = LONG(object_rect_.top / game_option_manager->GameCellSize);
	}

	void basicRender(HDC hdc, HDC backHDC);
public:
#pragma region GetterAndSetter

public:
	UINT object_id() const
	{
		return object_id_;
	}

	void set_object_id(UINT object_id)
	{
		object_id_ = object_id;
	}

	__declspec(property(get = object_id, put = set_object_id)) UINT ObjectId;

	UINT object_bitmap_id() const
	{
		return object_bitmap_id_;
	}

	void set_object_bitmap_id(UINT object_bitmap_id)
	{
		object_bitmap_id_ = object_bitmap_id;
	}

	__declspec(property(get = object_bitmap_id, put = set_object_bitmap_id)) UINT ObjectBitmapId;

	POINT object_pos() const
	{
		return object_pos_;
	}

	void set_object_pos(const POINT& object_pos)
	{
		object_pos_ = object_pos;
	}

	__declspec(property(get = object_pos, put = set_object_pos)) POINT ObjectPos;

	POINT_UCHAR object_width_height() const
	{
		return object_width_height_;
	}

	void set_object_width_height(const POINT_UCHAR& object_width_height)
	{
		object_width_height_ = object_width_height;
	}

	__declspec(property(get = object_width_height, put = set_object_width_height)) POINT_UCHAR ObjectWidthHeight;

	RECT object_rect() const
	{
		return object_rect_;
	}

	void set_object_rect(const RECT& object_rect)
	{
		object_rect_ = object_rect;
	}

	__declspec(property(get = object_rect, put = set_object_rect)) RECT ObjectRect;
#pragma endregion
	Object(UINT object_id, UINT object_bitmap_id, const UCHAR &posX, const UCHAR &posY, const UCHAR &width, const UCHAR &height, direction direction = dir_right)
		: object_id_(object_id),
		object_bitmap_id_(object_bitmap_id)
	{
		object_pos_.x = posX;
		object_pos_.y = posY;
		object_width_height_.x = width;
		object_width_height_.y = height;
		direction_ = direction;
		set_object_rect();
		render = &Object::basicRender;
	}
	Object() = default;
	virtual ~Object()
	{}

	//virtual void render(HDC hdc, HDC backHDC);
	void (Object::*render)(HDC hdc, HDC backHDC);
	virtual void update() { };
};

inline void Object::basicRender(HDC hdc, HDC backHDC)
{
	static BITMAP object_bitmap;
	static auto bitmap_manager = BitmapManager::GetInstance();
	GetObject(bitmap_manager->imageMap.find(ObjectBitmapId)->second, sizeof(BITMAP), &object_bitmap);
	if (direction_ == dir_right)
	{
		SelectObject(backHDC, bitmap_manager->imageMap.find(ObjectBitmapId)->second);
		TransparentBlt(hdc, ObjectRect.left, ObjectRect.top, ObjectRect.right - ObjectRect.left, ObjectRect.bottom - ObjectRect.top, backHDC, 0, 0, object_bitmap.bmWidth, object_bitmap.bmHeight, RGB(255, 0, 255));
	}
	else
	{
		HDC reverseHDC = CreateCompatibleDC(backHDC);
		SelectObject(reverseHDC, bitmap_manager->imageMap.find(ObjectBitmapId)->second);
		
		HBITMAP backHBIT = CreateCompatibleBitmap(hdc, object_bitmap.bmWidth, object_bitmap.bmHeight);
		SelectObject(backHDC, backHBIT);
		
		StretchBlt(backHDC, object_bitmap.bmWidth - 1, 0, -object_bitmap.bmWidth, object_bitmap.bmHeight, reverseHDC, 0, 0, object_bitmap.bmWidth, object_bitmap.bmHeight, SRCCOPY);
		TransparentBlt(hdc, ObjectRect.left, ObjectRect.top, ObjectRect.right - ObjectRect.left, ObjectRect.bottom - ObjectRect.top, backHDC, 0, 0, object_bitmap.bmWidth, object_bitmap.bmHeight, RGB(255, 0, 255));
		DeleteDC(reverseHDC);
		DeleteObject(backHBIT);
	}
	if(object_id_ == 3)
	{
		int object_left = (object_pos_.x + 3) * GameOptionManager::GetInstance()->GameCellSize;
		int object_top = (object_pos_.y +object_width_height_.y) * GameOptionManager::GetInstance()->GameCellSize;
		Rectangle(hdc, object_left, object_top, object_left + GameOptionManager::GetInstance()->GameCellSize, object_top + GameOptionManager::GetInstance()->GameCellSize);
	}
}

