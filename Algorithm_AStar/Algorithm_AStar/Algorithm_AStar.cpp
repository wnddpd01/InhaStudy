// Algorithm_AStar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

const int ways = 4;
const int way[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

NODE map[128][128];
POINT start_p = {0, 0};
POINT end_p = {0, 0};
size_t height = 0;
size_t width = 0;

void setFile()
{
	size_t height = 7;
	size_t width = 8;
	ofstream out("map.txt");
	static char number[4];
	sprintf(number, "%d ", height);
	out.write(number, sizeof(number));
	sprintf(number, "%d ", width);
	out.write(number, sizeof(number));
	char map[7][8] =
	{
		{ '0','0','0','0','0','0','0','0' },
		{ '0','X','X','X','0','X','X','0' },
		{ '0','0','0','0','0','X','X','0' },
		{ '0','S','0','X','0','X','X','F' },
		{ '0','0','0','X','0','0','0','0' },
		{ '0','0','0','X','0','0','0','0' },
		{ '0','0','0','0','0','0','0','0' },
	};
	static char newLine = '\n';
	out.write(&newLine, 1);
	for (size_t i = 0; i < 7; i++)
	{
		out.write(map[i], 8);
		out.write(&newLine, 1);
	}
	out.close();
}

void readFile(fstream &fstream)
{
	static char number[4];
	fstream.read(number, sizeof(number));
	height = atoi(number);
	fstream.read(number, sizeof(number));
	width = atoi(number);
	cout << height << " " << width << endl;
	static char tempMap[128];
	static char newLine = 0;
	fstream.read(&newLine, 1);
	for(int i = 0; i < height; i++)
	{
		memset(tempMap, 0, 128);
		fstream.read(tempMap, width);
		fstream.read(&newLine, 1);
		for (int j = 0; j < width; j++)
		{
			map[i][j].tile_state = tempMap[j];
			map[i][j].f_val = map[i][j].h_val = map[i][j].g_val = INT_MAX;
			map[i][j].y = i;
			map[i][j].x = j;
			if (tempMap[j] == 'S')
			{
				map[i][j].g_val = 0;
				start_p = { j, i };
			}
			else if (tempMap[j] == 'F')
				end_p = { j, i };
		}
		cout << tempMap << endl;
	}
}

size_t getDistanceToEndPoint(POINT &p)
{
	return abs(p.y - end_p.y) + abs(p.x - end_p.x);
}


size_t AStar()
{
	POINT cur_p = start_p;
	priorityQueue pq;
	push(&pq, &map[cur_p.y][cur_p.x]);
	while(pq.count != 0)
	{
		NODE * curNode = pop(&pq);
		cur_p = { curNode->x, curNode->y };
		for (size_t i = 0; i < ways; i++)
		{
			POINT search_p = { cur_p.x + way[i][0], cur_p.y + way[i][1] };
			if (map[search_p.y][search_p.x].tile_state == 'F')
				return map[cur_p.y][cur_p.x].g_val + 1;
			if (search_p.y > -1 && search_p.y < height &&search_p.x > -1 && search_p.x< width && map[search_p.y][search_p.x].tile_state == '0')
			{
				if(map[search_p.y][search_p.x].f_val == INT_MAX)
					push(&pq, &map[search_p.y][search_p.x]);
				size_t g_val = map[cur_p.y][cur_p.x].g_val + 1;
				if (g_val < map[search_p.y][search_p.x].g_val)
					map[search_p.y][search_p.x].g_val = g_val;
				map[search_p.y][search_p.x].h_val = getDistanceToEndPoint(search_p);
				map[search_p.y][search_p.x].f_val = map[search_p.y][search_p.x].g_val + map[search_p.y][search_p.x].h_val;

			}
		}
	}
	return INT_MAX;
}


int main()
{
	fstream mapFile("map.txt");
	if(mapFile.is_open() == false)
	{
		setFile();
		mapFile.open("map.txt");
	}
	readFile(mapFile);
	mapFile.close();
	cout << AStar() << endl;
    return 0;
}

