#pragma once
#include <Novice.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Map
{
public:
	inline static vector<vector<char>> _mapData1;

	static void LoadNovice();
	static void LoaclMapLoad(string name, vector<vector<char>>& mapData);
};

