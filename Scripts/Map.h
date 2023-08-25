#pragma once
#include <Novice.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Map
{
public:
	inline static int _mapValue1[3];//记录长度信息，width，height，minMapSize
	inline static vector<vector<char>> _mapData1;//记录地图信息，参数为行列分布
	inline static int _mapValue2[3];
	inline static vector<vector<char>> _mapData2;
	inline static int _mapValue_name[3];
	inline static vector<vector<char>> _mapData_name;
	inline static int _mapValue_help[3];
	inline static vector<vector<char>> _mapData_help;
	inline static int _mapValue_temp[3];
	inline static vector<vector<char>> _mapData_temp;

	static void LoadNovice();
	static void LoaclMapLoad(string name, vector<vector<char>>& mapData);

	//判断是否为可通过方块
	static bool IsThrough(vector<vector<char>> mapData, int line, int row);
};

