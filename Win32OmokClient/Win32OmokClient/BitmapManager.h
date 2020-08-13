#pragma once
class BitmapManager
{
	BitmapManager();
	HBITMAP m_HBitMapBoard;
	HBITMAP m_HBitWhtieStone;
	HBITMAP m_HBitBlackStone;
public:
	static BitmapManager * getInstance()
	{
		static BitmapManager instance;
		return &instance;
	}

	const HBITMAP * getHBitBoard();
	const HBITMAP * getHBitWhiteStone();
	const HBITMAP * getHBitBlackStone();
	~BitmapManager();
};

