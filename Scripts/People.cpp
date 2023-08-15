#include "People.h"

People::Vector2 People::ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY)
{
	float worldX = (float)(screenX + cameraX - 1920.f / 2);
	float worldY = (float)(screenY * -1 + cameraY - 1080.f / 2 + 1080);
	return Vector2{ worldX,worldY };
}

int People::MyTimers(int milli, int index)
{
	if (!_isTimeOpen[index]) {
		_timeStart[index] = clock();
		_isTimeOpen[index] = true;
	}
	_timeEnd[index] = clock();
	if (_timeEnd[index] - _timeStart[index] > milli) {
		_isTimeOpen[index] = false;
		return 1;
	}
	return 0;
}
