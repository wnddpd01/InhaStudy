#include "Object.h"


void Object::basicRender(HDC hdc, HDC backHDC)
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
}
