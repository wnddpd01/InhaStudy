#pragma once
#include <vector>
#include <Windows.h>

enum shortCut : UCHAR
{
	player1_move_left = 0, player1_move_right, player1_jump
};

class GameOptionManager
{
	GameOptionManager()
	{
		GameWidth = 1600;
		GameHeight = 900;
		HorizontalGridCount = 64;
		VerticalGridCount = 36;
		GameCellSize = GameWidth / HorizontalGridCount;
		Frame = 60;
		shortCutKeyList.push_back(VK_LEFT);
		shortCutKeyList.push_back(VK_RIGHT);
		shortCutKeyList.push_back(VK_UP);
	}
	UINT game_width_;
	UINT game_height_;
	UINT game_cell_size_;
	size_t horizontal_grid_count_;
	size_t vertical_grid_count_;
	UCHAR frame_;

#pragma region GetterAndSetter
public:
	UINT get_game_width() const
	{
		return game_width_;
	}

	void set_game_width(UINT game_width)
	{
		game_width_ = game_width;
	}

	__declspec(property(get = get_game_width, put = set_game_width)) UINT GameWidth;

	UINT get_game_height() const
	{
		return game_height_;
	}

	void set_game_height(UINT game_height)
	{
		game_height_ = game_height;
	}

	__declspec(property(get = get_game_height, put = set_game_height)) UINT GameHeight;

	UINT get_game_cell_size() const
	{
		return game_cell_size_;
	}

	void set_game_cell_size(UINT game_cell_size)
	{
		game_cell_size_ = game_cell_size;
	}

	__declspec(property(get = get_game_cell_size, put = set_game_cell_size)) UINT GameCellSize;

	size_t get_horizontal_grid_count() const
	{
		return horizontal_grid_count_;
	}

	void set_horizontal_grid_count(size_t horizontal_grid_count)
	{
		horizontal_grid_count_ = horizontal_grid_count;
	}

	__declspec(property(get = get_horizontal_grid_count, put = set_horizontal_grid_count)) size_t HorizontalGridCount;

	size_t get_vertical_grid_count() const
	{
		return vertical_grid_count_;
	}

	void set_vertical_grid_count(size_t vertical_grid_count)
	{
		vertical_grid_count_ = vertical_grid_count;
	}

	__declspec(property(get = get_vertical_grid_count, put = set_vertical_grid_count)) size_t VerticalGridCount;

	UCHAR get_frame() const
	{
		return frame_;
	}

	void set_frame(UCHAR frame)
	{
		frame_ = frame;
	}

	__declspec(property(get = get_frame, put = set_frame)) UCHAR Frame;

#pragma endregion

public:
	std::vector<UCHAR> shortCutKeyList;
	
	static GameOptionManager * GetInstance()
	{
		static GameOptionManager ins;
		return &ins;
	}

	~GameOptionManager()
	{
	}
};

