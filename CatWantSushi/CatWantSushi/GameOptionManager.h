#pragma once
#include <Windows.h>
class GameOptionManager
{
	GameOptionManager()
	{
		GameWidth = 1920;
		GameHeight = 1080;
		HorizontalGridCount = 64;
		VerticalGridCount = 36;
		GameCellSize = GameWidth / HorizontalGridCount;
		frame_ = 30;
	}
	UINT game_width_;
	UINT game_height_;
	UINT game_cell_size_;
	size_t horizontal_grid_count_;
	size_t vertical_grid_count_;
	UCHAR frame_;
#pragma region GetterAndSetter
public:
	UINT game_width() const
	{
		return game_width_;
	}

	void set_game_width(UINT game_width)
	{
		game_width_ = game_width;
	}

	__declspec(property(get = game_width, put = set_game_width)) UINT GameWidth;

	UINT game_height() const
	{
		return game_height_;
	}

	void set_game_height(UINT game_height)
	{
		game_height_ = game_height;
	}

	__declspec(property(get = game_height, put = set_game_height)) UINT GameHeight;

	UINT game_cell_size() const
	{
		return game_cell_size_;
	}

	void set_game_cell_size(UINT game_cell_size)
	{
		game_cell_size_ = game_cell_size;
	}

	__declspec(property(get = game_cell_size, put = set_game_cell_size)) UINT GameCellSize;

	size_t horizontal_grid_count() const
	{
		return horizontal_grid_count_;
	}

	void set_horizontal_grid_count(size_t horizontal_grid_count)
	{
		horizontal_grid_count_ = horizontal_grid_count;
	}

	__declspec(property(get = horizontal_grid_count, put = set_horizontal_grid_count)) size_t HorizontalGridCount;

	size_t vertical_grid_count() const
	{
		return vertical_grid_count_;
	}

	void set_vertical_grid_count(size_t vertical_grid_count)
	{
		vertical_grid_count_ = vertical_grid_count;
	}

	__declspec(property(get = vertical_grid_count, put = set_vertical_grid_count)) size_t VerticalGridCount;

	UCHAR frame() const
	{
		return frame_;
	}

	void set_frame(UCHAR frame)
	{
		frame_ = frame;
	}

	__declspec(property(get = frame, put = set_frame)) UCHAR Frame;
#pragma endregion
public :
	static GameOptionManager * GetInstance()
	{
		static GameOptionManager ins;
		return &ins;
	}

	~GameOptionManager()
	{
	}
};
