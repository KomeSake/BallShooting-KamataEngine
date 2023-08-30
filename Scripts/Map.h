#pragma once
#include <Novice.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Map
{
public:
	inline static int _mapValue1[5];//记录长度信息，width，height，minMapSize，玩家出生X，玩家出生Y
	inline static vector<vector<char>> _mapData1;//记录地图信息，参数为行列分布
	inline static int _mapValue2[5];
	inline static vector<vector<char>> _mapData2;
	inline static int _mapValue3[5];
	inline static vector<vector<char>> _mapData3;
	inline static int _mapValue4[5];
	inline static vector<vector<char>> _mapData4;
	inline static int _mapValue_name[5];
	inline static vector<vector<char>> _mapData_name;
	inline static int _mapValue_help[5];
	inline static vector<vector<char>> _mapData_help;
	inline static int _mapValue_start[5];
	inline static vector<vector<char>> _mapData_start;

	static void LoadNovice();
	static void LoaclMapLoad(string name, vector<vector<char>>& mapData);

	//判断是否为可通过方块
	static bool IsThrough(vector<vector<char>> mapData, int line, int row);
};

