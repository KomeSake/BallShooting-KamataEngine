#include "People.h"

People::Vector2 People::ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY)
{
	float worldX = (float)(screenX + cameraX - 1920.f / 2);
	float worldY = (float)(screenY * -1 + cameraY - 1080.f / 2 + 1080);
	return Vector2{ worldX,worldY };
}
