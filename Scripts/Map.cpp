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
	switch (mapData[line][row]) {
	case 'o':
		return true;
	case 'w':
		return false;
	case 'e':
		return true;
	case 'f':
		return true;
	}
	return false;
}
