#include "Map.h"

void Map::LoadNovice()
{
	_mapValue1[0] = 45 * 128;
	_mapValue1[1] = 25 * 128;
	_mapValue1[2] = 128;
	LoaclMapLoad("./Resources/Map/Map1.txt", _mapData1);
	_mapValue2[0] = 28 * 128;
	_mapValue2[1] = 25 * 128;
	_mapValue2[2] = 128;
	LoaclMapLoad("./Resources/Map/Map2.txt", _mapData2);
	_mapValue_name[0] = 30 * 128;
	_mapValue_name[1] = 25 * 128;
	_mapValue_name[2] = 128;
	LoaclMapLoad("./Resources/Map/Map_name.txt", _mapData_name);
	_mapValue_help[0] = 51 * 128;
	_mapValue_help[1] = 25 * 128;
	_mapValue_help[2] = 128;
	LoaclMapLoad("./Resources/Map/Map_help.txt", _mapData_help);
	_mapValue_temp[0] = 20 * 128;
	_mapValue_temp[1] = 20 * 128;
	_mapValue_temp[2] = 128;
	LoaclMapLoad("./Resources/Map/Map_temp.txt", _mapData_temp);
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
	case 'o'://地板
		return true;
	case 'w'://墙壁
		return false;
	case 'e'://敌人1，狗
		return true;
	case 'f'://敌人2，裂嘴兽
		return true;
	case 'U'://提示1
		return true;
	case '*'://虚空（会掉落）
		return true;
	case '/'://虚空墙壁（谨防任何东西超出地图最大范围）
		return false;
	}
	return false;
}
