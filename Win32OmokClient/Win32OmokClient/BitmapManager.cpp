#include "stdafx.h"
#include "BitmapManager.h"
#include "resource.h"
extern HINSTANCE hInst;

BitmapManager::BitmapManager()
{
	m_HBitMapBoard	 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOARD));
	m_HBitBlackStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BLACK_STONE));
	m_HBitWhtieStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_WHITE_STONE));
}


const HBITMAP * BitmapManager::getHBitBoard()
{
	return &m_HBitMapBoard;
}

const HBITMAP * BitmapManager::getHBitWhiteStone()
{
	return &m_HBitWhtieStone;
}

const HBITMAP * BitmapManager::getHBitBlackStone()
{
	return &m_HBitBlackStone;
}

BitmapManager::~BitmapManager()
{
}
