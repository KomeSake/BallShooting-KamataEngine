#include "Map.h"

void Map::LoadNovice()
{
	_mapValue1[0] = 55 * 128;
	_mapValue1[1] = 27 * 128;
	_mapValue1[2] = 128;
	_mapValue1[3] = int(3 * 128 - 128.f / 2);
	_mapValue1[4] = int(6 * 128 - 128.f / 2);
	LoaclMapLoad("./Resources/Map/Map1.txt", _mapData1);
	_mapValue2[0] = 28 * 128;
	_mapValue2[1] = 25 * 128;
	_mapValue2[2] = 128;
	_mapValue2[3] = int(4 * 128 - 128.f / 2);
	_mapValue2[4] = int(4 * 128 - 128.f / 2);
	LoaclMapLoad("./Resources/Map/Map2.txt", _mapData2);
	_mapValue3[0] = 28 * 128;
	_mapValue3[1] = 25 * 128;
	_mapValue3[2] = 128;
	_mapValue3[3] = int(4 * 128 - 128.f / 2);
	_mapValue3[4] = int(4 * 128 - 128.f / 2);
	LoaclMapLoad("./Resources/Map/Map3.txt", _mapData3);
	_mapValue4[0] = 28 * 128;
	_mapValue4[1] = 25 * 128;
	_mapValue4[2] = 128;
	_mapValue4[3] = int(4 * 128 - 128.f / 2);
	_mapValue4[4] = int(4 * 128 - 128.f / 2);
	LoaclMapLoad("./Resources/Map/Map4.txt", _mapData4);
	_mapValue_name[0] = 30 * 128;
	_mapValue_name[1] = 25 * 128;
	_mapValue_name[2] = 128;
	_mapValue_name[3] = int(4 * 128 - 128.f / 2);
	_mapValue_name[4] = int(4 * 128 - 128.f / 2);
	LoaclMapLoad("./Resources/Map/Map_name.txt", _mapData_name);
	_mapValue_help[0] = 56 * 128;
	_mapValue_help[1] = 25 * 128;
	_mapValue_help[2] = 128;
	_mapValue_help[3] = int(2 * 128 - 128.f / 2);
	_mapValue_help[4] = int(15 * 128 - 128.f / 2);
	LoaclMapLoad("./Resources/Map/Map_help.txt", _mapData_help);
	_mapValue_start[0] = 15 * 128;
	_mapValue_start[1] = 9 * 128;
	_mapValue_start[2] = 128;
	_mapValue_start[3] = int(7 * 128 - 128.f / 2);
	_mapValue_start[4] = int(2 * 128 - 128.f / 2);
	LoaclMapLoad("./Resources/Map/Map_start.txt", _mapData_start);
}

void Map::LoaclMapLoad(string name, vector<vector<char>>& mapData)
{
	ifstream file(name);

	if (!file.is_open()) {
		Novice::ConsolePrintf("File load is erro!!\n");
	}

	string line;
	vector<string> mapVector;
	while (getline(file, line)) {
		mapVector.push_back(line);
	}

	file.close();

	for (const string& row : mapVector) {
		vector<char> rowVector(row.begin(), row.end());
		mapData.push_back(rowVector);
	}
}

bool Map::IsThrough(vector<vector<char>> mapData, int line, int row)
{
	//首先要判断行列是否超过了容器的大小
	if (line > mapData.size()) {
		return false;
	}
	if (row > mapData[0].size()) {
		return false;
	}
	switch (mapData[line][row]) {
	case 'w'://墙壁
		return false;
	case '/'://虚空墙壁（谨防任何东西超出地图最大范围）
		return false;
		//可以穿过的东西多得多，所以只有不可穿过的才记录下来了
	}
	return true;
}
