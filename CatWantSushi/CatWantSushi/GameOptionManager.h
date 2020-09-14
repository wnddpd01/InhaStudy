#pragma once
#include <map>
#include <Windows.h>

using namespace std;
enum shortCut : UCHAR
{
	player1_attack = 0, player1_guard, player1_move_left, player1_move_right, player1_jump, player2_attack, player2_guard, player2_move_left, player2_move_right, player2_jump
};

class GameOptionManager
{
	GameOptionManager()
	{
		GameWidth = 1280;
		GameHeight = 720;
		Frame = 60;
		HorizontalGridCount = 64;
		VerticalGridCount = 36;
		GameCellSize = GameWidth / HorizontalGridCount;
		shortCutKeyList.insert(make_pair(player1_move_left, VK_LEFT));
		shortCutKeyList.insert(make_pair(player1_move_right, VK_RIGHT));
		shortCutKeyList.insert(make_pair(player1_jump, VK_UP));
		shortCutKeyList.insert(make_pair(player1_guard, VK_OEM_2));
		shortCutKeyList.insert(make_pair(player1_attack, VK_OEM_PERIOD));
		shortCutKeyList.insert(make_pair(player2_move_left, 'A'));
		shortCutKeyList.insert(make_pair(player2_move_right, 'D'));
		shortCutKeyList.insert(make_pair(player2_jump, 'W'));
		shortCutKeyList.insert(make_pair(player2_attack, 'G'));
		shortCutKeyList.insert(make_pair(player2_guard, 'H'));
	}
	UINT game_width_;
	UINT game_height_;
	LONG game_cell_size_;
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

	LONG get_game_cell_size() const
	{
		return game_cell_size_;
	}

	void set_game_cell_size(UINT game_cell_size)
	{
		game_cell_size_ = game_cell_size;
	}

	__declspec(property(get = get_game_cell_size, put = set_game_cell_size)) LONG GameCellSize;

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
	map<shortCut, UCHAR> shortCutKeyList;
	
	static GameOptionManager * GetInstance()
	{
		static GameOptionManager ins;
		return &ins;
	}

	~GameOptionManager()
	{
	}
};

