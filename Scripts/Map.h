#pragma once
#include <Novice.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Map
{
public:
	//记录地图信息，参数为行列分布
	inline static vector<vector<char>> _mapData1;

	static void LoadNovice();
	static void LoaclMapLoad(string name, vector<vector<char>>& mapData);

	//判断是否为可通过方块
	static bool IsThrough(vector<vector<char>> mapData, int line, int row);
};

